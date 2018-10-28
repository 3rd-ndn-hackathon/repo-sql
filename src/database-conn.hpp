#ifndef REPOSQL_DATABASE_CONN_HPP
#define REPOSQL_DATABASE_CONN_HPP

#include "datastore-record.hpp"

namespace reposql {

using DatabaseSelectCallback = std::function<void(const optional<DatastoreRecord>& record)>;
using DatabaseInsertCallback = std::function<void()>;
using DatabaseErrorCallback = std::function<void(int error)>;

std::string
toByteaHex(const uint8_t* s, size_t count);

std::string
toByteaHex(const ndn::Block& block, bool wantValueOnly = false);

/** \brief provides a connection to the database
 */
class DatabaseConn : noncopyable
{
public:
  DatabaseConn(boost::asio::io_service& io, std::string connstr);

  /** \brief execute a SELECT statement, and return the first row
   */
  void
  select(const std::string& where, bool wantDesc,
         const DatabaseSelectCallback& done, const DatabaseErrorCallback& error);

  /** \brief insert a record
   */
  void
  insert(const DatastoreRecord& record,
         const DatabaseInsertCallback& done, const DatabaseErrorCallback& error);

private:
  boost::asio::io_service& m_io;
  std::string m_connstr;
};

} // namespace reposql

#endif // REPOSQL_DATABASE_CONN_HPP
