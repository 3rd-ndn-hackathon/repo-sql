#include "database-conn.hpp"

namespace reposql {

static int
main(int argc, char** argv)
{
  boost::asio::io_service io;
  DatabaseConn db(io, "localhost", 5432, "ndnrepo", "postgres", "870212");

  while (std::cin.peek() != std::char_traits<char>::eof()) {
    Data data;
    try {
      data.wireDecode(ndn::Block::fromStream(std::cin));
    }
    catch (const ndn::tlv::Error& e) {
      std::cerr << "decode-error " << e.what() << std::endl;
      continue;
    }

    db.insert(DatastoreRecord(data),
      [=] {
        std::cout << data.getFullName() << std::endl;
      },
      [] (int err) {
        std::cerr << "insert-error " << err << std::endl;
      });
    io.poll();
  }
  io.run();
  return 0;
}

} // namespace reposql

int
main(int argc, char** argv)
{
  return reposql::main(argc, argv);
}
