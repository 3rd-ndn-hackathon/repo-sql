#ifndef REPOSQL_DATASTORE_RECORD_HPP
#define REPOSQL_DATASTORE_RECORD_HPP

#include "common.hpp"

namespace reposql {

/** \brief stores a digest of KeyLocator field
 */
typedef std::array<uint8_t, 32> KeyLocatorHash;

/** \brief represents a record in the datastore
 */
class DatastoreRecord
{
public:
  DatastoreRecord(const Data& data);

  const Name&
  getName() const;

  size_t
  getNameLen() const;

  const KeyLocatorHash&
  getKeyLocatorHash() const;

  const Data&
  getData() const;

private:
  const Data m_data;
};

} // namespace reposql

#endif // REPOSQL_DATASTORE_RECORD_HPP
