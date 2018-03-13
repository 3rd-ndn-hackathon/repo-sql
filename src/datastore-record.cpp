#include "datastore-record.hpp"
#include <ndn-cxx/util/sha256.hpp>

namespace reposql {

KeyLocatorHash
computeKeyLocatorHash(const KeyLocator& kl)
{
  const ndn::Block wire = kl.wireEncode();
  ndn::ConstBufferPtr d = ndn::util::Sha256::computeDigest(wire.wire(), wire.size());
  KeyLocatorHash h;
  BOOST_ASSERT(d->size() == h.size());
  std::copy(d->begin(), d->end(), h.begin());
  return h;
}

const KeyLocatorHash&
getEmptyKeyLocatorHash()
{
  static KeyLocatorHash s_empty{0};
  return s_empty;
}

DatastoreRecord::DatastoreRecord(const Data& data)
  : m_data(data)
  , m_hasKeyLocatorHash(false)
{
}

const Name&
DatastoreRecord::getName() const
{
  return m_data.getFullName();
}

size_t
DatastoreRecord::getNameLen() const
{
  return this->getName().size();
}

const KeyLocatorHash&
DatastoreRecord::getKeyLocatorHash() const
{
  if (!m_hasKeyLocatorHash) {
    const_cast<DatastoreRecord*>(this)->updateKeyLocatorHash();
  }
  return m_keyLocatorHash;
}

void
DatastoreRecord::updateKeyLocatorHash()
{
  m_hasKeyLocatorHash = true;
  const Signature& sig = m_data.getSignature();
  if (sig.hasKeyLocator()) {
    m_keyLocatorHash = computeKeyLocatorHash(sig.getKeyLocator());
  }
  else {
    m_keyLocatorHash = getEmptyKeyLocatorHash();
  }
}

const Data&
DatastoreRecord::getData() const
{
  return m_data;
}

} // namespace reposql
