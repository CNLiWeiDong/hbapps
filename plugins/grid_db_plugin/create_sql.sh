#!/usr/bin/env bash

host="127.0.0.1"
db="eos_trad"
user="eos_trad"
password="123456"
rootPassword="123456"

echo "clear the world"
mysql -h ${host} -u root -p${rootPassword} -e "DROP USER IF EXISTS '${user}'@'%';
DROP DATABASE IF EXISTS ${db};
CREATE USER '${user}'@'%'IDENTIFIED BY '${password}';
CREATE DATABASE ${db} CHARACTER SET utf8 COLLATE utf8_general_ci;
GRANT ALL PRIVILEGES ON ${db}.* TO '${user}'@'%';"

echo "init db ${db} on ${host}"
mysql -b -h ${host} -u ${user} -p${password} -D ${db} -e "source ./table.sql"

echo "done"
