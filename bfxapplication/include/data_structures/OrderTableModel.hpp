#include <map>
#include <quickfix/Message.h>

namespace FIX{
/*
OrderTableRow is for providing outstanding orders that are not filled or partially filled.
 */
struct OrderTableRow{
    FIX::ClOrdID aClOrdID;
    FIX::Side aSide;
    FIX::TransactTime aTime;
    FIX::OrdType aType;
};

using OrderTableModel = std::map<std::string, OrderTableRow>;

};