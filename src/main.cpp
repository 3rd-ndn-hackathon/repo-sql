#include "database-conn.hpp"
#include "query-processor.hpp"

namespace reposql {

static int
main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Usage: repo-sql $(cat connstr.txt)" << std::endl;
    return 2;
  }

  boost::asio::io_service io;
  DatabaseConn db(io, argv[1]);

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
