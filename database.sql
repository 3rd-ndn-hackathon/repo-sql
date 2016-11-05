CREATE TABLE datastore (
    name bytea CONSTRAINT name_pk PRIMARY KEY,
    namelen smallint,
    keylocatorhash bytea,
    data bytea
    );

CREATE INDEX name_namelen ON datastore (name, namelen);
