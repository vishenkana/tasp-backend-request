--------------------------------------------------------------------------------
DO $$
DECLARE
    schema text;
    name text;
BEGIN
    FOR schema IN
        SELECT nspname
        FROM pg_catalog.pg_namespace
        WHERE nspname NOT IN ('information_schema', 'pg_catalog')
            AND nspname NOT LIKE 'pg_toast'
            AND nspname NOT LIKE 'pg_temp'
    LOOP
        EXECUTE 'ALTER SCHEMA ' || quote_ident(schema) || ' OWNER TO admin_tasp';

        EXECUTE 'ALTER DEFAULT PRIVILEGES FOR ROLE oper_tasp IN SCHEMA ' || quote_ident(schema) ||
            ' GRANT SELECT, UPDATE, INSERT, DELETE ON TABLES TO oper_tasp';

        EXECUTE 'ALTER DEFAULT PRIVILEGES FOR ROLE oper_tasp IN SCHEMA ' || quote_ident(schema) ||
            ' GRANT EXECUTE ON FUNCTIONS TO oper_tasp';

        EXECUTE 'ALTER DEFAULT PRIVILEGES FOR ROLE oper_tasp IN SCHEMA ' || quote_ident(schema) ||
            ' GRANT USAGE ON SEQUENCES TO oper_tasp';

        EXECUTE 'GRANT USAGE ON SCHEMA ' || quote_ident(schema) || ' TO oper_tasp';

        EXECUTE 'GRANT SELECT, UPDATE, INSERT, DELETE ON ALL TABLES IN SCHEMA ' || quote_ident(schema) || ' TO oper_tasp';
        EXECUTE 'GRANT EXECUTE ON ALL FUNCTIONS IN SCHEMA ' || quote_ident(schema) || ' TO oper_tasp';
        EXECUTE 'GRANT USAGE ON ALL SEQUENCES IN SCHEMA ' || quote_ident(schema) || ' TO oper_tasp';

        EXECUTE 'REVOKE EXECUTE ON ALL FUNCTIONS IN SCHEMA ' || quote_ident(schema) || ' FROM public';

        FOR name IN
            SELECT format('%I.%I', schemaname, tablename)
            FROM pg_tables
            WHERE schemaname = quote_ident(schema)
        LOOP
            EXECUTE 'ALTER TABLE ' || name || ' OWNER TO admin_tasp';
            EXECUTE 'GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE ' || name || ' TO oper_tasp';
        END LOOP;

        FOR name IN
            SELECT format('%I.%I(%s)', ns.nspname, p.proname, oidvectortypes(p.proargtypes))
            FROM pg_proc p INNER JOIN pg_namespace ns ON (p.pronamespace = ns.oid)
            WHERE ns.nspname = quote_ident(schema)
        LOOP
            EXECUTE 'ALTER FUNCTION ' || name || '  OWNER TO admin_tasp';
        END LOOP;

        FOR name IN
            SELECT format('%I.%I', schemaname, viewname)
            FROM pg_catalog.pg_views
            WHERE schemaname NOT IN ('pg_catalog', 'information_schema')
                AND schemaname = quote_ident(schema)
        LOOP
            EXECUTE 'ALTER TABLE ' || name || '  OWNER TO admin_tasp';
        END LOOP;
    END LOOP;
END $$;
