# Installation

## Dependencies

Environment: Ubuntu 16.04

```
sudo apt-get install postgresql-9.5 libpq-dev libndn-cxx-dev nfd
```

## Database Setup

Install:

```
sudo -u postgres createdb ndnrepo
sudo -u postgres psql -d postgres -c "CREATE USER ndnrepo PASSWORD '0000';"
sudo -u postgres psql -d ndnrepo -c "GRANT ALL ON ALL TABLES IN SCHEMA public TO ndnrepo;"
echo 'postgres://ndnrepo:0000@localhost/ndnrepo' > connstr.txt
psql $(cat connstr.txt) -f database.sql
```

Rollback:

```
sudo -u postgres dropdb ndnrepo
sudo -u postgres psql -d postgres -c "DROP USER ndnrepo;"
```
