/**
 * @file
 * @brief Реализация API получения данных для формирования заявки.
 */
#ifndef TASP_REQUESTS_PARAMS_HPP_
#define TASP_REQUESTS_PARAMS_HPP_

#include <memory>

#include <tasp/http.hpp>
#include <tasp/http/client.hpp>
#include <tasp/microservice.hpp>

namespace tasp::requests
{

/**
 * @brief Класс реализации API получения данных для формирования заявки.
 *
 */
class Params final
{
public:
    /**
     * @brief Конструктор.
     *
     * Добавляет в микросервис обработчики API-запросов получения данных
     * для формирования заявки.
     *
     * @param service Указатель на микросервис
     */
    explicit Params(const MicroService *service) noexcept;

    /**
     * @brief Деструктор.
     */
    ~Params() noexcept = default;

    Params(const Params &) = delete;
    Params(Params &&) = delete;
    Params &operator=(const Params &) = delete;
    Params &operator=(Params &&) = delete;

private:
    /**
     * @brief Запрос параметров для формирования заявки.
     *
     * @param request Запрос
     * @param response Ответ
     */
    void Data(const http::Request &request, http::Response &response) noexcept;

    /**
     * @brief Запрос информации о космических аппаратах.
     */
    std::unique_ptr<http::Client> satellites_{nullptr};
};

}  // namespace tasp::requests

#endif  // TASP_REQUESTS_PARAMS_HPP_
