#ifndef FIX42_ORDERCANCELBATCHREJECT_H
#define FIX42_ORDERCANCELBATCHREJECT_H

#include "custom_messages/FixCoinbaseFields.h"
#include "quickfix/fix42/Message.h"
#include <quickfix/FixFields.h>

namespace FIX42 {

class OrderCancelBatchReject : public Message {
public:
  OrderCancelBatchReject() : Message(MsgType()) {}
  OrderCancelBatchReject(const FIX::Message &m) : Message(m) {}
  OrderCancelBatchReject(const Message &m) : Message(m) {}
  OrderCancelBatchReject(const OrderCancelBatchReject &m) : Message(m) {}
  static FIX::MsgType MsgType() { return FIX::MsgType("U5"); }

  FIELD_SET(*this, FIX::BatchID);
  FIELD_SET(*this, FIX::Text);
};

} // namespace FIX42
#endif