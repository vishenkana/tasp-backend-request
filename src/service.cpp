#include "service.hpp"

namespace tasp::requests
{

/*------------------------------------------------------------------------------
    Service
------------------------------------------------------------------------------*/
Service::Service(int argc, const char **argv) noexcept
: MicroService(argc, argv)
{
}

//------------------------------------------------------------------------------
Service::~Service() noexcept = default;

}  // namespace tasp::requests
