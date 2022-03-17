/**
 * @file OrderTableModel.hpp
 * @brief a collection of structures/classes for storing orders in engine.
 */
#include <map>
#include <quickfix/FixFields.h>
#include <quickfix/Message.h>
#include <quickfix/fix42/ExecutionReport.h>

namespace FIX {
/*
OrderRow is for providing outstanding orders that are not filled or partially
filled.
 */

struct OrderRow {
  FIX::ClOrdID aClOrdID;
};

using OrderTableModel = std::map<FIX::OrderID, FIX::OrderRow>;

}; // namespace FIX