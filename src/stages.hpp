/**
 * @file
 * @brief Реализация API запроса информации о этапах.
 */
#ifndef TASP_REQUESTS_STAGES_HPP_
#define TASP_REQUESTS_STAGES_HPP_

#include <tasp/db/pg.hpp>
#include <tasp/http.hpp>
#include <tasp/microservice.hpp>

namespace tasp::requests
{

/**
 * @brief Класс реализации API запроса информации о этапах.
 *
 */
class Stages final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param service Указатель на микросервис
     */
    explicit Stages(const MicroService *service) noexcept;

    /**
     * @brief Деструктор.
     */
    ~Stages() noexcept = default;

    Stages(const Stages &) = delete;
    Stages(Stages &&) = delete;
    Stages &operator=(const Stages &) = delete;
    Stages &operator=(Stages &&) = delete;

private:
    /**
     * @brief Список стадий.
     *
     * @param request Запрос
     * @param response Ответ
     */
    void List(const http::Request &request, http::Response &response);

    /**
     * @brief Пул подключений к БД.
     */
    db::pg::ConnectionPool &db_pool_{db::pg::ConnectionPool::Instance()};
};

}  // namespace tasp::requests

#endif  // TASP_REQUESTS_STAGES_HPP_