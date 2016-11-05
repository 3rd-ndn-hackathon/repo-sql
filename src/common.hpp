#ifndef REPOSQL_COMMON_HPP
#define REPOSQL_COMMON_HPP

#include <memory>
#include <ndn-cxx/util/backports.hpp>

#include <boost/noncopyable.hpp>

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/lp/nack.hpp>

namespace reposql {

using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using ndn::make_unique;
using ndn::optional;
using ndn::nullopt;

using boost::noncopyable;

using ndn::Face;
using ndn::Name;
using ndn::Interest;
using ndn::Data;
using ndn::lp::Nack;

} // namespace reposql

#endif // REPOSQL_COMMON_HPP
