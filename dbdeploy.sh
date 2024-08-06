#!/bin/bash

SCRIPT_DIR="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

PASSWORD="12345678"

IP="127.0.0.1"
PORT="5432"

while getopts "i:p:a:" opt
do
    case $opt in
        i) IP=$OPTARG;;
        p) PORT=$OPTARG;;
        a) PASSWORD=$OPTARG;;
    esac
done

SQL="SELECT pg_terminate_backend(pid) FROM pg_stat_activity WHERE pid <> pg_backend_pid() AND datname='tasp';"
psql postgresql://postgres:$PASSWORD@$IP:$PORT/postgres -c "${SQL}" 1>/dev/null

psql postgresql://postgres:$PASSWORD@$IP:$PORT/tasp -c "DROP SCHEMA IF EXISTS requests CASCADE;" 1>/dev/null

(
    cd $SCRIPT_DIR/src/sql
    psql postgresql://postgres:$PASSWORD@$IP:$PORT/postgres -f deploy.sql 1>/dev/null
)

psql postgresql://postgres:$PASSWORD@$IP:$PORT/postgres -c "ALTER USER tasp WITH LOGIN ENCRYPTED PASSWORD '12345678';"
