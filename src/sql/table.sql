--------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS requests.requests
(
    id serial NOT NULL PRIMARY KEY,
    satellite_number_id smallint NOT NULL,
    reception_point_id smallint NOT NULL,
    comment text,
    need_request boolean NOT NULL DEFAULT false,
    stage requests.stages NOT NULL DEFAULT 'creation',
    status requests.stages_status NOT NULL DEFAULT 'pending', 
    creation_date timestamp without time zone NOT NULL DEFAULT now()
);

COMMENT ON TABLE requests.requests IS 'Основная информация по заявкам';

COMMENT ON COLUMN requests.requests.id IS 'Идентификатор';
COMMENT ON COLUMN requests.requests.satellite_number_id IS 'Идентификатор аппарата';
COMMENT ON COLUMN requests.requests.reception_point_id IS 'Идентификатор пункта приема';
COMMENT ON COLUMN requests.requests.comment IS 'Комментарий';
COMMENT ON COLUMN requests.requests.need_request IS 'Флаг необходимости формирования заявки на выдачу';
COMMENT ON COLUMN requests.requests.stage IS 'Этап заявки';
COMMENT ON COLUMN requests.requests.status IS 'Статус заявки';
COMMENT ON COLUMN requests.requests.creation_date IS 'Дата создания';

--------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS requests.information
(
    id serial NOT NULL PRIMARY KEY,
    requests_id integer NOT NULL REFERENCES requests.requests ON DELETE CASCADE ON UPDATE CASCADE,
    information_type_id text NOT NULL,
    delivery_status boolean NOT NULL DEFAULT false,
    delivery_date timestamp without time zone DEFAULT null,
    UNIQUE(requests_id, information_type_id)
);

COMMENT ON TABLE requests.information IS 'Запрошенная информация заявки';

COMMENT ON COLUMN requests.information.id IS 'Идентификатор';
COMMENT ON COLUMN requests.information.requests_id IS 'Идентификатор заявки';
COMMENT ON COLUMN requests.information.information_type_id IS 'Идентификатор типа информации';
COMMENT ON COLUMN requests.information.delivery_status IS 'Статус получения данных';
COMMENT ON COLUMN requests.information.delivery_date IS 'Дата получения данных';

--------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS requests.destination_point
(
    id serial NOT NULL PRIMARY KEY,
    requests_id integer NOT NULL REFERENCES requests.requests ON DELETE CASCADE ON UPDATE CASCADE,
    destination_point_id text NOT NULL,
    write_status boolean NOT NULL DEFAULT false,
    write_date timestamp without time zone DEFAULT null,
    UNIQUE(requests_id, destination_point_id)
);

COMMENT ON TABLE requests.destination_point IS 'Пункты предназначения информации в заявке';

COMMENT ON COLUMN requests.destination_point.id IS 'Идентификатор';
COMMENT ON COLUMN requests.destination_point.requests_id IS 'Идентификатор заявки';
COMMENT ON COLUMN requests.destination_point.destination_point_id IS 'Идентификатор пункта предназначения информации';
COMMENT ON COLUMN requests.destination_point.write_status IS 'Статус записи информации';
COMMENT ON COLUMN requests.destination_point.write_date IS 'Дата записи информации';

--------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS requests.stages_statuses
(
    id serial NOT NULL PRIMARY KEY,
    requests_id integer NOT NULL REFERENCES requests.requests ON DELETE CASCADE ON UPDATE CASCADE,
    stage requests.stages NOT NULL,
    status requests.stages_status NOT NULL DEFAULT 'pending',
    comment text,
    date timestamp without time zone NOT NULL DEFAULT now(),
    UNIQUE(requests_id, stage, status)
);

COMMENT ON TABLE requests.stages_statuses IS 'Статусы этапов';

COMMENT ON COLUMN requests.stages_statuses.id IS 'Идентификатор';
COMMENT ON COLUMN requests.stages_statuses.requests_id IS 'Идентификатор заявки';
COMMENT ON COLUMN requests.stages_statuses.stage IS 'Этап';
COMMENT ON COLUMN requests.stages_statuses.status IS 'Статус этапа';
COMMENT ON COLUMN requests.stages_statuses.comment IS 'Комментарий';
COMMENT ON COLUMN requests.stages_statuses.date IS 'Дата изменения статуса';

--------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS requests.stages_spec
(
    name requests.stages NOT NULL PRIMARY KEY,
    description text NOT NULL
);

COMMENT ON TABLE requests.stages_spec IS 'Описание статусов этапов';

COMMENT ON COLUMN requests.stages_spec.name IS 'Название';
COMMENT ON COLUMN requests.stages_spec.description IS 'Описание';

--------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS requests.statistics
(
    name text NOT NULL PRIMARY KEY,
    value integer NOT NULL
);

COMMENT ON TABLE requests.stages_spec IS 'Статистика по заявкам';

COMMENT ON COLUMN requests.stages_spec.name IS 'Название параметра';
COMMENT ON COLUMN requests.stages_spec.description IS 'Значение параметра';
