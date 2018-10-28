#ifndef REPOSQL_QUERY_PROCESSOR_HPP
#define REPOSQL_QUERY_PROCESSOR_HPP

#include "database-conn.hpp"

namespace reposql {

/** \brief implements an Interest query service
 */
class QueryProcessor : noncopyable
{
public:
  QueryProcessor(Face& face, DatabaseConn& db);

  /** \brief register a top-level prefix with NFD
   */
  void
  addTopPrefix(const Name& prefix);

private:
  void
  processInterest(const Interest& interest);

  std::string
  makeWhereCondition(const Interest& interest);

private:
  Face& m_face;
  DatabaseConn& m_db;
};

} // namespace reposql

#endif // REPOSQL_QUERY_PROCESSOR_HPP
