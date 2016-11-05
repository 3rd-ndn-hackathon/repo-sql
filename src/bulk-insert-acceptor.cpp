#include "bulk-insert-acceptor.hpp"

namespace reposql {

BulkInsertAcceptor::BulkInsertAcceptor(boost::asio::io_service& io, boost::asio::ip::tcp::endpoint ep,
                                       DatabaseConn& db)
  : m_acceptor(io, ep)
  , m_db(db)
{
  KeyChain keyChain;
  shared_ptr<Data> data;

  data = make_shared<Data>("/repo/P/A/1");
  keyChain.sign(*data);
  db.insert(DatastoreRecord(*data), std::bind([]{}), std::bind([]{}));

  data = make_shared<Data>("/repo/P/A/2");
  keyChain.sign(*data);
  db.insert(DatastoreRecord(*data), std::bind([]{}), std::bind([]{}));

  data = make_shared<Data>("/repo/P/B/1");
  keyChain.sign(*data);
  db.insert(DatastoreRecord(*data), std::bind([]{}), std::bind([]{}));

  data = make_shared<Data>("/repo/P/B/2");
  keyChain.sign(*data);
  db.insert(DatastoreRecord(*data), std::bind([]{}), std::bind([]{}));
}

} // namespace reposql
