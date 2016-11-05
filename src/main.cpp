#include "database-conn.hpp"
#include "query-processor.hpp"
#include "bulk-insert-acceptor.hpp"

namespace reposql {

static int
main(int argc, char** argv)
{
  boost::asio::io_service io;
  DatabaseConn db(io, "localhost", 5432, "ndnrepo", "postgres", "870212");

  Face face(io);
  QueryProcessor qp(face, db);
  qp.addTopPrefix("/repo");

  boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::loopback(), 7376);
  BulkInsertAcceptor bulk(io, ep, db);

  io.run();
  return 0;
}

} // namespace reposql

int
main(int argc, char** argv)
{
  return reposql::main(argc, argv);
}
