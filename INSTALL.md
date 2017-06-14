# Installation

## Dependencies

    sudo apt-get install postgresql-9.3  ndn-cxx-dev nfd

## Database Setup

    sudo -u postgres createdb ndnrepo
    sudo -u postgres psql -d ndnrepo -f database.sql
