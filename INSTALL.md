# Installation

## Dependencies

Environment: Ubuntu 16.04

    sudo apt-get install postgresql-9.5 libpq-dev libndn-cxx-dev nfd

## Database Setup

    sudo -u postgres createdb ndnrepo
    sudo -u postgres psql -d ndnrepo -f database.sql
    sudo -u postgres psql -d postgres -c "ALTER ROLE postgres PASSWORD '870212'"
