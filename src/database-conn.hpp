#ifndef REPOSQL_DATABASE_CONN_HPP
#define REPOSQL_DATABASE_CONN_HPP

#include "datastore-record.hpp"

namespace reposql {

using DatabaseSelectCallback = std::function<void(const optional<DatastoreRecord>& record)>;
using DatabaseInsertCallback = std::function<void()>;
using DatabaseErrorCallback = std::function<void(int error)>;

std::string
toByteaHex(const uint8_t* s, size_t count);

/** \brief provides a connection to the database
 */
class DatabaseConn : noncopyable
{
public:
  DatabaseConn(boost::asio::io_service& io,
               const std::string& host, uint16_t port, const std::string& dbname,
               const std::string& username, const std::string& password);

  /** \brief execute a SELECT statement, and return the first row
   */
  void
  select(const std::string& stmt, const DatabaseSelectCallback& done, const DatabaseErrorCallback& error);

  /** \brief insert a record
   */
  void
  insert(const DatastoreRecord& record, const DatabaseInsertCallback& done, const DatabaseErrorCallback& error);
};

} // namespace reposql

#endif // REPOSQL_DATABASE_CONN_HPP
