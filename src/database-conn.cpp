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

std::string
toByteaHex(const ndn::Block& block, bool wantValueOnly)
{
  if (wantValueOnly) {
    return toByteaHex(block.value(), block.value_size());
  }
  else {
    return toByteaHex(block.wire(), block.size());
  }
}

DatabaseConn::DatabaseConn(boost::asio::io_service& io,
                           const std::string& host, uint16_t port, const std::string& dbname,
                           const std::string& username, const std::string& password)
  : m_io(io)
  , m_connstr("host=" + host + " port=" + std::to_string(port) + " dbname=" + dbname +
              " user=" + username + " password=" + password)
{
  NDN_LOG_INFO("connstr=" << m_connstr);
}

void
DatabaseConn::select(const std::string& where, bool wantDesc,
                     const DatabaseSelectCallback& done, const DatabaseErrorCallback& error)
{
  std::ostringstream b;
  b << "SELECT data, LENGTH(data) FROM datastore WHERE "
    << where
    << " ORDER BY name "
    << (wantDesc ? "DESC" : "ASC")
    << " LIMIT 1";
  std::string stmt = b.str();
  NDN_LOG_DEBUG(stmt);

  auto conn = postgrespp::Connection::create(m_io, m_connstr.c_str());
  conn->queryParams(stmt.c_str(),
    [conn, done, error] (const boost::system::error_code& ec, postgrespp::Result r) {
      NDN_LOG_TRACE("ec=" << ec << " status=" << r.getStatus() << " error=" << r.getErrorMessage() << " rows=" << r.rows());
      // if (ec || r.getStatus() != 1) {
      //   error(r.getStatus());
      //   return;
      // }
      if (r.rows() == 0) {
        done(nullopt);
        return;
      }
      BOOST_VERIFY(r.next());
      const char* dataHex = r.get<char*>(0);
      int dataSize = std::atoi(r.get<char*>(1));
      BOOST_ASSERT(std::strlen(dataHex) == 2 + dataSize * 2);
      BOOST_ASSERT(dataHex[0] == '\\');
      BOOST_ASSERT(dataHex[1] == 'x');
      shared_ptr<const ndn::Buffer> dataBuf = ndn::fromHex(std::string(dataHex + 2, dataSize * 2));
      Data data;
      try {
        ndn::Block block(dataBuf);
        data.wireDecode(block);
      }
      catch (const ndn::tlv::Error& e) {
        NDN_LOG_WARN("decode-error " << e.what());
        error(-3);
        return;
      }
      done(DatastoreRecord(data));
    });
}

void
DatabaseConn::insert(const DatastoreRecord& record,
                     const DatabaseInsertCallback& done, const DatabaseErrorCallback& error)
{
  std::ostringstream b;
  b << "INSERT INTO datastore (name, namelen, keylocatorhash, data) VALUES ("
    << toByteaHex(record.getName().wireEncode(), true) << ", "
    << record.getNameLen() << ", "
    << toByteaHex(record.getKeyLocatorHash().data(), record.getKeyLocatorHash().size()) << ", "
    << toByteaHex(record.getData().wireEncode()) << ")";
  std::string stmt = b.str();
  NDN_LOG_DEBUG(stmt);

  auto conn = postgrespp::Connection::create(m_io, m_connstr.c_str());
  conn->queryParams(stmt.c_str(),
    [conn, done, error] (const boost::system::error_code& ec, postgrespp::Result r) {
      NDN_LOG_TRACE("ec=" << ec << " status=" << r.getStatus() << " error=" << r.getErrorMessage());
      if (ec || r.getStatus() != 1) {
        error(r.getStatus());
        return;
      }
      done();
    });
}

} // namespace reposql
