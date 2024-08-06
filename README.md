# ПК ТА: Микросервис ведения заявок

[[_TOC_]]

## Общие сведения

### Назначение

Микросервис предназначен для ведения заявок на извлечение данных из архива и
их записи на переносные носители информации.

### Документация API

- [OpenAPI](./docs/openapi.yml)

### Используемые библиотеки

- libtasp-microservice - библиотека для создания микросервисов ПК ТА.
- libtasp-bd-pg - библиотека для работы с СУБД PostgreSQL ПК ТА.

## Сборка и компиляция

### Компиляция

Для компиляции необходимы следующие компоненты:

- компилятор Clang 11;
- генератор файлов сборки CMake;
- система сборки Ninja;
- поиск зависимостей pkg-config;
- библиотека ПК ТА libtasp-microservice;
- библиотека ПК ТА libtasp-bd-pg.

#### Загрузка submodule

Для загрузки **submodule/cmake** необходимо выполнить команды:

```sh
git submodule update --init --recursive
git submodule update --recursive --remote
```

#### Инструкция по компиляции

Для компиляции необходимо выполнить последовательность действий:

1. Установить необходимые для компиляции пакеты:

    ```sh
    sudo apt install -y --no-install-recommends \
        clang-11 \
        llvm-11-dev \
        cmake \
        ninja-build \
        pkg-config
    ```

2. Выполнить компиляцию:

    ```sh
    (
        mkdir build
        cd build
        cmake ..
        ninja
    )
    ```

    >**Примечание**:
    >
    > - Для компиляции в режиме DEBUG использовать: -DCMAKE_BUILD_TYPE=Debug;
    > - Для компиляции без ccache использовать: -DUSE_CCACHE=OFF.

#### Результаты компиляции

Файлы будут расположены в **build/bin**.

## Развертывание БД

Для развертывания БД необходимо выполнить команду:

```sh
bash dbdeploy.sh
```