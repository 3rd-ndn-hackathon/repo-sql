#include "datastore-record.hpp"

namespace reposql {

DatastoreRecord::DatastoreRecord(const Data& data)
  : m_data(data)
{
}

const Name&
DatastoreRecord::getName() const
{
  return m_data.getFullName();
}

const Data&
DatastoreRecord::getData() const
{
  return m_data;
}

} // namespace reposql
