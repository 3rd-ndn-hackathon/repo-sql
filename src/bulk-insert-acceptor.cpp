#include "bulk-insert-acceptor.hpp"

namespace reposql {

BulkInsertAcceptor::BulkInsertAcceptor(boost::asio::io_service& io, boost::asio::ip::tcp::endpoint ep,
                                       DatabaseConn& db)
  : m_acceptor(io, ep)
  , m_db(db)
{
  KeyChain keyChain;
  auto data = make_shared<Data>("/P/A/1");
  keyChain.sign(*data);

  DatastoreRecord record(*data);
  db.insert(record, std::bind([]{}), std::bind([]{}));
}

} // namespace reposql
