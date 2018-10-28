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

std::string
QueryProcessor::makeWhereCondition(const Interest& interest)
{
  std::ostringstream b;

  const Name& name = interest.getName();
  BOOST_ASSERT(name.size() > 0); // ndn:/ is disallowed
  b << "name>=" << toByteaHex(name.wireEncode(), true)
    << " AND name<" << toByteaHex(name.getSuccessor().wireEncode(), true);

  return b.str();
}

} // namespace reposql
