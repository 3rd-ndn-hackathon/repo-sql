#include "bulk-insert-acceptor.hpp"

namespace reposql {

BulkInsertAcceptor::BulkInsertAcceptor(boost::asio::io_service& io, boost::asio::ip::tcp::endpoint ep,
                                       DatabaseConn& db)
  : m_acceptor(io, ep)
  , m_db(db)
{
}

} // namespace reposql
