#pragma once

#include "petuum_ps/consistency/abstract_consistency_controller.hpp"
#include "petuum_ps/oplog/oplog.hpp"
#include "petuum_ps/util/vector_clock_mt.hpp"
#include <utility>
#include <vector>
#include <cstdint>
#include <atomic>

namespace petuum {

class SSPConsistencyController : public AbstractConsistencyController {
public:
  SSPConsistencyController(const TableInfo& info,
    int32_t table_id,
    ProcessStorage& process_storage,
    TableOpLog& oplog,
    const AbstractRow* sample_row);

  // Check freshness; make request and block if too stale or row_id not found
  // in storage.
  void Get(int32_t row_id, RowAccessor* row_accessor);

  // Return immediately.
  void Inc(int32_t row_id, int32_t column_id, const void* delta);

  void BatchInc(int32_t row_id, const int32_t* column_ids, const void* updates, 
    int32_t num_updates);

private:  // private methods
  // Fetch row_id from process_storage_ and check freshness against
  // stalest_iter. Return true if found and fresh, false otherwise.
  bool FetchFreshFromProcessStorage(int32_t row_id, int32_t stalest_iter,
      RowAccessor* row_accessor);

private:
  int32_t table_id_;

  // SSP staleness parameter.
  int32_t staleness_;
};

}  // namespace petuum