#include "request_params.hpp"

using std::make_unique;
using std::string;
using std::string_view;

namespace tasp::requests
{

using http::Client;
using http::Request;
using http::Response;

/*------------------------------------------------------------------------------
    Params
------------------------------------------------------------------------------*/
Params::Params(const MicroService *service) noexcept
{
    satellites_ =
        make_unique<Client>("reference", "/satellites", Request::Method::Get);

    service->AddHandler(Request::Method::Get, "/params", this, &Params::Data);
}

//------------------------------------------------------------------------------
void Params::Data([[maybe_unused]] const Request &request,
                  Response &response) noexcept
{
    auto satellites = satellites_->Send();

    auto json = satellites->Data()->Get<Json::Value>();
    for (auto &element : json["data"])
    {
        string satellite_id = element["number"].asString();

        auto get_info = [&](string_view name)
        {
            const string path =
                "/satellites/" + satellite_id + "/" + name.data();
            const http::Client reference(
                "reference", path, Request::Method::Get);
            auto res = reference.Send();
            element[name.data()] = res->Data()->Get<Json::Value>()["data"];
        };

        get_info("information_types");
        get_info("reception_points");
        get_info("destination_points");
    }

    response.Data()->Set(json);
}

}  // namespace tasp::requests
