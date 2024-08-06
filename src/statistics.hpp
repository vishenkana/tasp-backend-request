/**
 * @file
 * @brief Реализация API запроса статистики по заявкам.
 */
#ifndef TASP_REQUESTS_STATISTICS_HPP_
#define TASP_REQUESTS_STATISTICS_HPP_

#include <tasp/db/pg.hpp>
#include <tasp/http.hpp>
#include <tasp/microservice.hpp>

namespace tasp::requests
{

/**
 * @brief Класс реализации API запроса статистики по заявкам.
 *
 */
class Statistics final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param service Указатель на микросервис
     */
    explicit Statistics(const MicroService *service) noexcept;

    /**
     * @brief Деструктор.
     */
    ~Statistics() noexcept = default;

    /**
     * @brief Изменение статистики по этапу.
     *
     * @param stage Название этапа
     * @param sign Знак
     */
    static void ModifyStage(std::string_view stage,
                            std::string_view sign = "+") noexcept;

    Statistics(const Statistics &) = delete;
    Statistics(Statistics &&) = delete;
    Statistics &operator=(const Statistics &) = delete;
    Statistics &operator=(Statistics &&) = delete;

private:
    /**
     * @brief Основная статистика заявок.
     *
     * @param request Запрос
     * @param response Ответ
     */
    void Basic(const http::Request &request, http::Response &response);

    /**
     * @brief Пересчет статистики по заявкам.
     *
     * Запрос нужен для синхронизации данных, если были изменения в БД.
     *
     * @param request Запрос
     * @param response Ответ
     */
    void Sync(const http::Request &request, http::Response &response);

    /**
     * @brief Пул подключений к БД.
     */
    db::pg::ConnectionPool &db_pool_{db::pg::ConnectionPool::Instance()};
};

}  // namespace tasp::requests

#endif  // TASP_REQUESTS_STATISTICS_HPP_