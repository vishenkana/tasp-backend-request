#include "statistics.hpp"

using std::string;
using std::string_view;

using tasp::db::pg::Query;

namespace tasp::requests
{

using http::Request;
using http::Response;

/*------------------------------------------------------------------------------
    Statistics
------------------------------------------------------------------------------*/
Statistics::Statistics(const MicroService *service) noexcept
{
    service->AddHandler(
        Request::Method::Get, "/stats", this, &Statistics::Basic);
    service->AddHandler(
        Request::Method::Post, "/stats/sync", this, &Statistics::Sync);
}

//------------------------------------------------------------------------------
void Statistics::ModifyStage(string_view stage, string_view sign) noexcept
{
    auto conn = db::pg::ConnectionPool::Instance().GetConnection();

    static Query sql{
        "UPDATE requests.statistics SET value = value {} 1 WHERE name = '{}'"};
    auto result = conn->Exec(sql, sign, stage);
}

//------------------------------------------------------------------------------
void Statistics::Basic([[maybe_unused]] const Request &request,
                       Response &response)
{
    Json::Value root;

    auto conn = db_pool_.GetConnection();
    auto result = conn->Exec(Query("SELECT * FROM requests.statistics"));
    for (const auto &row : *result)
    {
        root[row.Value("name")] = std::stoi(row.Value("value"));
    }

    response.Data()->Set(root);
}

//------------------------------------------------------------------------------
void Statistics::Sync([[maybe_unused]] const Request &request,
                      Response &response)
{
    auto conn = db_pool_.GetConnection();
    auto transaction = conn->BeginTransaction();

    auto update_stage = [&](string_view stage, bool open = false)
    {
        Query sql{"SELECT count(*) "
                  "FROM requests.requests "
                  "WHERE status IN ('pending', 'warning')"};

        if (!open)
        {
            sql += " and stage = '{}'";
        }

        auto result = conn->Exec(sql, stage);
        if (!result->Status())
        {
            throw std::exception{};
        }

        sql = "UPDATE requests.statistics SET value = {} WHERE name = '{}'";
        result = conn->Exec(sql, result->Value("count"), stage);
        if (!result->Status())
        {
            throw std::exception{};
        }
    };

    try
    {
        update_stage("open", true);
        update_stage("waiting_write");
        update_stage("closing");
    }
    catch (std::exception &exp)
    {
        transaction->Rollback();
        response.SetError(Response::Code::Internal,
                          "Ошибка добавления информации в БД");
        return;
    }
}

}  // namespace tasp::requests
