/**
 * @file
 * @brief Реализация API работы с заявкой.
 */
#ifndef TASP_REQUESTS_REQUEST_HPP_
#define TASP_REQUESTS_REQUEST_HPP_

#include <memory>

#include <tasp/db/pg.hpp>
#include <tasp/http.hpp>
#include <tasp/http/client.hpp>
#include <tasp/microservice.hpp>

namespace tasp::requests
{

/**
 * @brief Класс реализации API работы с заявкой.
 *
 */
class Request final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param service Указатель на микросервис
     */
    explicit Request(const MicroService *service) noexcept;

    /**
     * @brief Деструктор.
     */
    ~Request() noexcept = default;

    Request(const Request &) = delete;
    Request(Request &&) = delete;
    Request &operator=(const Request &) = delete;
    Request &operator=(Request &&) = delete;

private:
    /**
     * @brief Запрос добавления новой заявки.
     *
     * @param request Запрос
     * @param response Ответ
     */
    void Create(const http::Request &request, http::Response &response);

    /**
     * @brief Список заявок.
     *
     * @param request Запрос
     * @param response Ответ
     */
    void List(const http::Request &request, http::Response &response);

    /**
     * @brief Добавление нового статуса этапа.
     *
     * Подключение к БД передается для выполнения операции в одной транзакции.
     *
     * @param conn Подключения к БД
     * @param request_id Идентификатор заявки
     * @param stage Этап
     * @param status Статус
     * @param comment Комментарий
     */
    static void AddStageStatus(const std::shared_ptr<db::pg::Connection> &conn,
                               std::string_view request_id,
                               std::string_view stage,
                               std::string_view status = "pending",
                               std::string_view comment = {});

    /**
     * @brief Проверка необходимости создания заявки на выдачу информации из
     * архива.
     *
     * @param information Массив типов информации
     *
     * @return Флаг необходимости создания заявки.
     */
    [[nodiscard]] static bool IsNeedRequest(
        const Json::Value &information) noexcept;

    /**
     * @brief Пул подключений к БД.
     */
    db::pg::ConnectionPool &db_pool_{db::pg::ConnectionPool::Instance()};
};

}  // namespace tasp::requests

#endif  // TASP_REQUESTS_REQUEST_HPP_
