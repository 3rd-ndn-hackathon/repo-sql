#include "query-processor.hpp"

namespace reposql {

NDN_LOG_INIT(QueryProcessor);

QueryProcessor::QueryProcessor(Face& face, DatabaseConn& db)
  : m_face(face)
  , m_db(db)
{
}

void
QueryProcessor::addTopPrefix(const Name& prefix)
{
  NDN_LOG_INFO("register-prefix " << prefix);
  m_face.setInterestFilter(prefix,
    bind(&QueryProcessor::processInterest, this, _2),
    [] (const Name&, const std::string& err) {
      NDN_LOG_ERROR("register-prefix-error " << err);
    });
}

void
QueryProcessor::processInterest(const Interest& interest)
{
  NDN_LOG_DEBUG("interest " << interest);

  if (interest.getChildSelector() == 1) {
    BOOST_ASSERT_MSG(false, "not implemented");
  }
  else {
    m_db.select(this->makeWhereCondition(interest), false,
      [=] (const optional<DatastoreRecord>& record) {
        if (!record) {
          NDN_LOG_TRACE("not-found");
          ///\todo send Nack
          return;
        }
        NDN_LOG_TRACE("found " << record->getName());
        m_face.put(record->getData());
      },
      [=] (int err) {
        NDN_LOG_WARN("db.select err=" << err);
      });
  }
}

std::string
QueryProcessor::makeWhereCondition(const Interest& interest)
{
  std::ostringstream b;

  BOOST_ASSERT(interest.getName().size() > 0); // ndn:/ is disallowed
  b << "name>=" << toByteaHex(interest.getName().wireEncode(), true)
    << " AND name<" << toByteaHex(interest.getName().getSuccessor().wireEncode(), true);

  if (interest.getMinSuffixComponents() >= 0) {
    b << " AND namelen>=" << (interest.getName().size() + interest.getMinSuffixComponents());
  }

  if (interest.getMaxSuffixComponents() >= 0) {
    b << " AND namelen<=" << (interest.getName().size() + interest.getMaxSuffixComponents());
  }

  if (!interest.getPublisherPublicKeyLocator().empty()) {
    KeyLocatorHash klh = computeKeyLocatorHash(interest.getPublisherPublicKeyLocator());
    b << " AND keylocatorhash=" << toByteaHex(klh.data(), klh.size());
  }

  if (!interest.getExclude().empty()) {
    BOOST_ASSERT_MSG(false, "not implemented");
  }

  return b.str();
}

std::string
QueryProcessor::makeWhereCondition2(const Interest& interest, const Name& maxDataName)
{
  std::ostringstream b;

  BOOST_ASSERT(interest.getName().size() < maxDataName.size());
  Name prefix = maxDataName.getPrefix(interest.getName().size() + 1);
  b << "name>=" << toByteaHex(prefix.wireEncode(), true)
    << " AND name<" << toByteaHex(prefix.getSuccessor().wireEncode(), true);

  if (interest.getMinSuffixComponents() >= 0) {
    b << " AND namelen>=" << (interest.getName().size() + interest.getMinSuffixComponents());
  }

  if (interest.getMaxSuffixComponents() >= 0) {
    b << " AND namelen<=" << (interest.getName().size() + interest.getMaxSuffixComponents());
  }

  if (!interest.getPublisherPublicKeyLocator().empty()) {
    KeyLocatorHash klh = computeKeyLocatorHash(interest.getPublisherPublicKeyLocator());
    b << " AND keylocatorhash=" << toByteaHex(klh.data(), klh.size());
  }

  return b.str();
}

} // namespace reposql
