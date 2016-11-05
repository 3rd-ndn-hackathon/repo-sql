#ifndef REPOSQL_BULK_INSERT_ACCEPTOR_HPP
#define REPOSQL_BULK_INSERT_ACCEPTOR_HPP

#include "database-conn.hpp"

namespace reposql {

/** \brief implements a bulk insert service
 */
class BulkInsertAcceptor : noncopyable
{
public:
  BulkInsertAcceptor(boost::asio::io_service& io, boost::asio::ip::tcp::endpoint ep,
                     DatabaseConn& db);

private:
  boost::asio::ip::tcp::acceptor m_acceptor;
  DatabaseConn& m_db;
};

} // namespace reposql

#endif // REPOSQL_BULK_INSERT_ACCEPTOR_HPP
