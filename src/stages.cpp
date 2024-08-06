#include "stages.hpp"

using std::string;

using tasp::db::pg::Query;

namespace tasp::requests
{

using http::Request;
using http::Response;

/*------------------------------------------------------------------------------
    Stages
------------------------------------------------------------------------------*/
Stages::Stages(const MicroService *service) noexcept
{
    service->AddHandler(Request::Method::Get, "/stages", this, &Stages::List);
}

//------------------------------------------------------------------------------
void Stages::List([[maybe_unused]] const Request &request, Response &response)
{
    auto conn = db_pool_.GetConnection();
    Query sql{"SELECT * FROM requests.stages_spec"};
    auto result = conn->Exec(sql);

    response.Data()->Set(result->JsonValue());
}

}  // namespace tasp::requests
