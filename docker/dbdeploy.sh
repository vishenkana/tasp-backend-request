#!/bin/bash

(
    cd /sql/requests/
    psql -v ON_ERROR_STOP=1 -f deploy.sql
)
psql -v ON_ERROR_STOP=1 -c "ALTER USER tasp WITH LOGIN ENCRYPTED PASSWORD '12345678';"