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
    FIX::ClOrdID aClOrdID;

    FIX::Side aSide;
    FIX::TransactTime aTime;
    FIX::OrdType aOrdType;
    FIX::OrdStatus aOrdStatus;

    std::optional<FIX::Symbol> aSymbol;
    std::optional<FIX::OrderQty> aOrderQty;
    std::optional<FIX::CumQty> aCumQty;
    std::optional<FIX::LeavesQty> aLeavesQty;
};

using OrderTableModel = std::map<FIX::ClOrdID, FIX::OrderRow>;

};