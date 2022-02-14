/**
 * @file OrderTableModel.hpp
 * @brief a collection of structures/classes for storing orders in engine.
 */
#include <map>
#include <quickfix/Message.h>

namespace FIX{
/*
OrderRow is for providing outstanding orders that are not filled or partially filled.
 */

struct OrderRow{
    FIX::Side aSide;
    FIX::TransactTime aTime;
    FIX::OrdType aOrdType;
    std::optional<FIX::Symbol> aSymbol;
    std::optional<FIX::OrderQty> aOrderQty;
};

using OrderTableModel = std::map<std::string, FIX::OrderRow>;

};