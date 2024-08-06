/**
 * @file
 * @brief Реализация микросервиса ведения заявок.
 */
#ifndef TASP_SERVICE_HPP_
#define TASP_SERVICE_HPP_

#include <tasp/microservice.hpp>

#include "request.hpp"
#include "request_params.hpp"
#include "stages.hpp"
#include "statistics.hpp"

namespace tasp::requests
{

/**
 * @brief Главный класс микросервиса.
 *
 */
class Service final : public MicroService
{
public:
    /**
     * @brief Конструктор.
     *
     * @param argc Количество аргументов
     * @param argv Аргументы
     */
    Service(int argc, const char **argv) noexcept;

    /**
     * @brief Деструктор.
     */
    ~Service() noexcept override;

    Service(const Service &) = delete;
    Service(Service &&) = delete;
    Service &operator=(const Service &) = delete;
    Service &operator=(Service &&) = delete;

private:
    /**
     * @brief Данные по этапам.
     */
    Stages stages_{this};

    /**
     * @brief API для работы с заявками.
     */
    Request request_{this};

    /**
     * @brief Данные для формирования заявки.
     */
    Params params_{this};

    /**
     * @brief Статистика по заявкам.
     */
    Statistics statistics_{this};
};

}  // namespace tasp::requests

#endif  // TASP_SERVICE_HPP_
