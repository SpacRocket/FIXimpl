#ifndef FIX_COINBASE_FIELDS_H
#define FIX_COINBASE_FIELDS_H

#include "FixCoinbaseFieldNumbers.h"
#include "quickfix/Field.h"

#undef Yield

namespace FIX {
DEFINE_CHAR(CancelOrdersOnDisconnect);
DEFINE_CHAR(DropCopyFlag);
DEFINE_CHAR(SelfTradePrevention);
DEFINE_STRING(BatchID);
} // namespace FIX
#endif // FIX_COINBASE_FIELDS_H
