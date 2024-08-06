cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/submodule/cmake")

include(DetectOs)
include(SetupEnvironment)

project(tasp-requests LANGUAGES CXX)

include(SetupCompileOptions)
include(SetupHardening)

file(GLOB_RECURSE SOURCES
  ./src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

include(Dependency)

pkg_check_modules(JSONCPP REQUIRED jsoncpp)

tasp_check_modules(tasp-common)
tasp_check_modules(tasp-microservice)
tasp_check_modules(tasp-curl)
tasp_check_modules(tasp-db-pg)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        jsoncpp
    PRIVATE
        ${TASP-COMMON_LDFLAGS}
        ${TASP-MICROSERVICE_LDFLAGS}
        ${TASP-CURL_LDFLAGS}
        ${TASP-DB-PG_LDFLAGS}
)

include(SetupInstall)
