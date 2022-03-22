#ifndef FIX42_ORDERCANCELBATCHREQUEST_H
#define FIX42_ORDERCANCELBATCHREQUEST_H

#include "custom_messages/FixCoinbaseFields.h"
#include "quickfix/fix42/Message.h"
#include <quickfix/FixFields.h>

namespace FIX42 {

class OrderCancelBatchRequest : public Message {
public:
  OrderCancelBatchRequest() : Message(MsgType()) {}
  OrderCancelBatchRequest(const FIX::Message &m) : Message(m) {}
  OrderCancelBatchRequest(const Message &m) : Message(m) {}
  OrderCancelBatchRequest(const OrderCancelBatchRequest &m) : Message(m) {}
  static FIX::MsgType MsgType() { return FIX::MsgType("U4"); }

  FIELD_SET(*this, FIX::BatchID);
  FIELD_SET(*this, FIX::NoOrders);
  FIELD_SET(*this, FIX::OrigClOrdID);
  FIELD_SET(*this, FIX::Symbol);
  FIELD_SET(*this, FIX::OrderID);
  FIELD_SET(*this, FIX::ClOrdID);
};

} // namespace FIX42
#endif