# PostgreSQL-based NDN repository

repo-sql is an NDN repository using a PostgreSQL database as backing storage.
Its design goal focuses on scalability, in that it transforms the unfamiliar problem of scaling an NDN repository into the familiar problem of scaling a database.

This codebase contains two programs:

* `repo-sql` is a service that answers Interests by querying the database.
* `repo-sql-insert` is a tool that reads Data packets from stdin and inserts them into the database.
