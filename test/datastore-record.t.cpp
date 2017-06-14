#include "datastore-record.hpp"
#include <ndn-cxx/security/signing-helpers.hpp>
#include "test/tests-common.hpp"

namespace reposql {
namespace tests {

BOOST_AUTO_TEST_SUITE(TestDatastoreRecord)

BOOST_AUTO_TEST_CASE(NoKeyLocator)
{
  KeyChain keyChain;
  auto data = make_shared<Data>("/P/A/1");
  keyChain.sign(*data, ndn::security::signingWithSha256());

  DatastoreRecord record(*data);
  BOOST_CHECK_EQUAL(record.getName(), data->getFullName());
  BOOST_CHECK_EQUAL(record.getNameLen(), 4);
  BOOST_CHECK(record.getKeyLocatorHash() == getEmptyKeyLocatorHash());
  BOOST_CHECK(record.getData() == *data);
}

BOOST_AUTO_TEST_CASE(HasKeyLocator)
{
  KeyChain keyChain;
  auto data1 = make_shared<Data>("/P/A/1");
  auto data2 = make_shared<Data>("/P/A/2");
  keyChain.sign(*data1);
  keyChain.sign(*data2);

  DatastoreRecord record1(*data1);
  BOOST_CHECK_EQUAL(record1.getName(), data1->getFullName());
  BOOST_CHECK_EQUAL(record1.getNameLen(), 4);
  BOOST_CHECK(record1.getKeyLocatorHash() != getEmptyKeyLocatorHash());
  BOOST_CHECK(record1.getData() == *data1);

  DatastoreRecord record2(*data2);
  BOOST_CHECK(record1.getKeyLocatorHash() == record2.getKeyLocatorHash());
}

BOOST_AUTO_TEST_SUITE_END() // TestDatastoreRecord

} // namespace tests
} // namespace reposql
