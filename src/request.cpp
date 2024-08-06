#include "request.hpp"

#include <tasp/http/request.hpp>

#include "statistics.hpp"

using std::shared_ptr;
using std::string;
using std::string_view;

using tasp::db::pg::Query;

using namespace std::literals;

namespace tasp::requests
{

using db::pg::Connection;
using http::Response;

/*------------------------------------------------------------------------------
    Request
------------------------------------------------------------------------------*/
Request::Request(const MicroService *service) noexcept
{
    service->AddHandler(
        http::Request::Method::Post, "/", this, &Request::Create);

    service->AddHandler(http::Request::Method::Get, "/", this, &Request::List);
}

//------------------------------------------------------------------------------
void Request::Create([[maybe_unused]] const http::Request &request,
                     Response &response)
{
    auto data = request.Data()->Get<Json::Value>();

    shared_ptr<Connection> conn = db_pool_.GetConnection();
    auto transaction = conn->BeginTransaction();

    auto need_request = IsNeedRequest(data["information"]);

    Query sql{"INSERT INTO requests.requests "
              "VALUES (DEFAULT, {}, {}, '{}', '{}') "
              "RETURNING id"};
    auto result = conn->Exec(sql,
                             data["satellite"],
                             data["reception_point"],
                             data["comment"],
                             need_request);
    if (!result->Status())
    {
        response.SetError(Response::Code::Internal,
                          "Ошибка добавления заявки в БД");
        return;
    }

    auto id_json = result->JsonValue();
    auto request_id = Json::Path(".data[0].id").resolve(id_json).asString();

    auto add_array = [&](string_view table, string_view key)
    {
        for (const auto &element : data[key.data()])
        {
            Query sql{"INSERT INTO requests.{} "
                      "VALUES (DEFAULT, '{}', '{}') "};
            result = conn->Exec(sql, table, request_id, element.asString());
            if (!result->Status())
            {
                throw std::exception{};
            }
        }
    };

    try
    {
        add_array("information", "information");
        add_array("destination_point", "destination_points");
        AddStageStatus(conn, request_id, "creation");
    }
    catch (std::exception &exp)
    {
        transaction->Rollback();
        response.SetError(Response::Code::Internal,
                          "Ошибка добавления информации в БД");
        return;
    }

    Statistics::ModifyStage("open");

    response.Data()->Set(id_json);
    response.SetCode(Response::Code::Created);
}

//------------------------------------------------------------------------------
void Request::AddStageStatus(const shared_ptr<Connection> &conn,
                             string_view request_id,
                             string_view stage,
                             string_view status,
                             string_view comment)
{
    Query sql{"INSERT INTO requests.stages_statuses "
              "VALUES (DEFAULT, {}, '{}', '{}', '{}') "};

    auto result = conn->Exec(sql, request_id, stage, status, comment);
    if (!result->Status())
    {
        throw std::exception{};
    }
}

//------------------------------------------------------------------------------
void Request::List([[maybe_unused]] const http::Request &request,
                   Response &response)
{
    auto conn = db_pool_.GetConnection();

    Query sql{"SELECT req.id, satellite_number_id, reception_point_id, "
              "creation_date, need_request, "
              "array_agg(DISTINCT information_type_id) AS information,"
              "array_agg(DISTINCT destination_point_id) AS destination_points "
              "FROM requests.requests AS req "
              "JOIN requests.information ON "
              "requests.information.requests_id = req.id "
              "JOIN requests.destination_point ON "
              "requests.destination_point.requests_id = req.id "
              "GROUP BY req.id "
              "ORDER BY req.id ASC"};

    sql.ParseUrlParams(request.Uri());

    auto result = conn->Exec(sql)->JsonValue();

    const http::Client reference("reference", "/");

    for (auto &element : result["data"])
    {
        string path =
            "/satellites/" + element["satellite_number_id"].asString();
        reference.Request()->Uri()->ChangePath(path);
        auto res = reference.Send();

        element.removeMember("satellite_number_id");
        element["satellite"] = Json::Path(".data[0].name")
                                   .resolve(res->Data()->Get<Json::Value>());

        path = "/reception_points/" + element["reception_point_id"].asString();
        reference.Request()->Uri()->ChangePath(path);
        res = reference.Send();

        element.removeMember("reception_point_id");
        element["reception_point"] =
            Json::Path(".data[0].description")
                .resolve(res->Data()->Get<Json::Value>());

        for (auto &information : element["information"])
        {
            path = "/information_type/" + information.asString();
            reference.Request()->Uri()->ChangePath(path);
            res = reference.Send();

            information = Json::Path(".data[0].short")
                              .resolve(res->Data()->Get<Json::Value>());
        }

        for (auto &destination_point : element["destination_points"])
        {
            path = "/destination_points/" + destination_point.asString();
            reference.Request()->Uri()->ChangePath(path);
            res = reference.Send();

            destination_point = Json::Path(".data[0].description")
                                    .resolve(res->Data()->Get<Json::Value>());
        }

        Query sql{
            "SELECT stage AS name, max(status) as status, max(date) as date "
            "FROM requests.stages_statuses "
            "WHERE requests_id = {} "
            "GROUP BY stage "
            "ORDER BY stage ASC"};
        element["stages"] = conn->Exec(sql, element["id"])->JsonValue()["data"];
    }

    response.Data()->Set(result);
}

//------------------------------------------------------------------------------
bool Request::IsNeedRequest(const Json::Value &information) noexcept
{
    const http::Client reference("reference", "/");
    for (const auto &element : information)
    {
        auto path = "/information_type/" + element.asString();
        reference.Request()->Uri()->ChangePath(path);
        auto res = reference.Send();

        auto need_request = Json::Path(".data[0].need_request")
                                .resolve(res->Data()->Get<Json::Value>());
        if (need_request.asBool())
        {
            return true;
        }
    }
    return false;
}

}  // namespace tasp::requests
