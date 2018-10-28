#ifndef REPOSQL_COMMON_HPP
#define REPOSQL_COMMON_HPP

#include <memory>
#include <ndn-cxx/util/backports.hpp>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include <ndn-cxx/data.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/lp/nack.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/security/signing-helpers.hpp>
#include <ndn-cxx/util/logger.hpp>

namespace reposql {

using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using ndn::optional;
using ndn::nullopt;

using boost::noncopyable;

using ndn::Name;
using ndn::Interest;
using ndn::Data;
using ndn::lp::Nack;
using ndn::Face;
using ndn::KeyChain;

} // namespace reposql

#endif // REPOSQL_COMMON_HPP
