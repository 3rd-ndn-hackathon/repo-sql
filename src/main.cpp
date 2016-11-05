#include "database-conn.hpp"
#include "query-processor.hpp"

namespace reposql {

static int
main(int argc, char** argv)
{
  boost::asio::io_service io;
  DatabaseConn db(io, "localhost", 5432, "ndnrepo", "postgres", "870212");

  Face face(io);
  QueryProcessor qp(face, db);
  qp.addTopPrefix("/repo");

  io.run();
  return 0;
}

} // namespace reposql

int
main(int argc, char** argv)
{
  return reposql::main(argc, argv);
}
