#include "database-conn.hpp"

namespace reposql {

static int
main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Usage: repo-sql-insert $(cat connstr.txt) < data.bin" << std::endl;
    return 2;
  }

  boost::asio::io_service io;
  DatabaseConn db(io, argv[1]);

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
