#include "common.hpp"
#include <ndn-cxx/util/io.hpp>

namespace reposql {

static int
main(int argc, char** argv)
{
  if (argc != 4) {
    std::cerr << "Usage: repo-sql-datagen /prefix min max > data.bin" << std::endl;
    return 2;
  }

  const Name prefix(argv[1]);
  const int min = std::atoi(argv[2]);
  const int max = std::atoi(argv[3]);

  KeyChain keyChain;
  auto si = ndn::security::signingWithSha256();
  for (int i = min; i <= max; ++i) {
    Name name = prefix;
    name.append(std::to_string(i));
    Data data(name);
    keyChain.sign(data, si);
    ndn::io::save(data, std::cout, ndn::io::NO_ENCODING);
  }

  return 0;
}

} // namespace reposql

int
main(int argc, char** argv)
{
  return reposql::main(argc, argv);
}
