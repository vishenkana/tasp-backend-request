--------------------------------------------------------------------------------
SET client_min_messages TO WARNING;

--------------------------------------------------------------------------------
DO $$
BEGIN
    IF NOT EXISTS(
        SELECT
        FROM pg_catalog.pg_roles
        WHERE rolname = 'admin_tasp'
    ) THEN
        CREATE ROLE admin_tasp;
    END IF;

    ALTER ROLE admin_tasp SET client_encoding = 'UTF8';
    COMMENT ON ROLE admin_tasp IS 'Администраторы БД ПК ТА';
END $$;

DO $$
BEGIN
    IF NOT EXISTS(
        SELECT
        FROM pg_catalog.pg_roles
        WHERE rolname = 'oper_tasp'
    ) THEN
        CREATE ROLE oper_tasp;
    END IF;

    ALTER ROLE oper_tasp SET client_encoding='UTF8';
    COMMENT ON ROLE oper_tasp IS 'Пользователи ПК ТА';
END $$;

--------------------------------------------------------------------------------
DO $$
BEGIN
    IF NOT EXISTS(
        SELECT
        FROM pg_catalog.pg_roles
        WHERE rolname = 'tasp'
    ) THEN
        CREATE ROLE tasp WITH LOGIN;
    END IF;

    ALTER ROLE tasp SET client_encoding = 'UTF8';
    COMMENT ON ROLE tasp IS 'Тех. пользователь ПК ТА';

    IF NOT EXISTS(
        SELECT
        FROM information_schema.applicable_roles
        WHERE grantee = 'tasp' AND role_name = 'oper_tasp'
    ) THEN
        GRANT oper_tasp TO tasp;
    END IF;
END $$;

--------------------------------------------------------------------------------
SELECT 'CREATE DATABASE tasp'
WHERE NOT EXISTS (SELECT FROM pg_database WHERE datname = 'tasp')\gexec

ALTER DATABASE tasp OWNER TO admin_tasp;
GRANT CONNECT, TEMPORARY ON DATABASE tasp TO oper_tasp;
REVOKE ALL ON DATABASE tasp FROM public;

COMMENT ON DATABASE tasp IS 'БД ПК ТА';
