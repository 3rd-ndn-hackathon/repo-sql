#include "datastore-record.hpp"
#include <ndn-cxx/security/signing-helpers.hpp>
#include "test/tests-common.hpp"

namespace reposql {
namespace tests {

BOOST_AUTO_TEST_SUITE(TestDatastoreRecord)

BOOST_AUTO_TEST_CASE(Basic)
{
  KeyChain keyChain;
  auto data = make_shared<Data>("/P/A/1");
  keyChain.sign(*data, ndn::security::signingWithSha256());

  DatastoreRecord record(*data);
  BOOST_CHECK_EQUAL(record.getName(), data->getFullName());
  BOOST_CHECK(record.getData() == *data);
}

BOOST_AUTO_TEST_SUITE_END() // TestDatastoreRecord

} // namespace tests
} // namespace reposql
