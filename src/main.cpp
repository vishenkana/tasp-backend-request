#include "service.hpp"

int main(int argc, const char **argv)
{
    const tasp::requests::Service service{argc, argv};
    return service.Exec();
}
