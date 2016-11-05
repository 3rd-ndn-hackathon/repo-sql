#include "database-conn.hpp"
#include <ndn-cxx/util/string-helper.hpp>
#include "postgrespp/connection.hpp"
#include "postgrespp/result.hpp"
#include "postgrespp/utility.hpp"

namespace reposql {

NDN_LOG_INIT(DatabaseConn);

std::string
toByteaHex(const uint8_t* s, size_t count)
{
  return "E'\\\\x" + ndn::toHex(s, count) + "'";
}

DatabaseConn::DatabaseConn(boost::asio::io_service& io,
                           const std::string& host, uint16_t port, const std::string& dbname,
                           const std::string& username, const std::string& password)
{
}

void
DatabaseConn::select(const std::string& where, bool wantDesc,
                     const DatabaseSelectCallback& done, const DatabaseErrorCallback& error)
{
  std::ostringstream b;
  b << "SELECT * FROM datastore WHERE "
    << where
    << " ORDER BY name "
    << (wantDesc ? "DESC" : "ASC")
    << " LIMIT 1";
  std::string stmt = b.str();
  NDN_LOG_DEBUG(stmt);
}

void
DatabaseConn::insert(const DatastoreRecord& record,
                     const DatabaseInsertCallback& done, const DatabaseErrorCallback& error)
{
  std::ostringstream b;
  b << "INSERT INTO datastore (name, namelen, keylocatorhash, data) VALUES ("
    << toByteaHex(record.getName().wireEncode().wire(), record.getName().wireEncode().size()) << ", "
    << record.getNameLen() << ", "
    << toByteaHex(record.getKeyLocatorHash().data(), record.getKeyLocatorHash().size()) << ", "
    << toByteaHex(record.getData().wireEncode().wire(), record.getData().wireEncode().size()) << ")";
  std::string stmt = b.str();
  NDN_LOG_DEBUG(stmt);
}

} // namespace reposql
