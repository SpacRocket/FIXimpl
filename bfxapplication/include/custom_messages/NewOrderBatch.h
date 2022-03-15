#ifndef FIX42_NEWORDERBATCH_H
#define FIX42_NEWORDERBATCH_H

#include "custom_messages/FixCoinbaseFields.h"
#include "quickfix/fix42/Message.h"
#include <quickfix/FixFields.h>

namespace FIX42 {

class NewOrderBatch : public Message {
public:
  NewOrderBatch() : Message(MsgType()) {}
  NewOrderBatch(const FIX::Message &m) : Message(m) {}
  NewOrderBatch(const Message &m) : Message(m) {}
  NewOrderBatch(const NewOrderBatch &m) : Message(m) {}
  static FIX::MsgType MsgType() { return FIX::MsgType("U6"); }

  FIELD_SET(*this, FIX::BatchID);
  FIELD_SET(*this, FIX::NoOrders);
  FIELD_SET(*this, FIX::ClOrdID);
  FIELD_SET(*this, FIX::Symbol);
  FIELD_SET(*this, FIX::Side);
  FIELD_SET(*this, FIX::Price);
  FIELD_SET(*this, FIX::OrderQty);
  FIELD_SET(*this, FIX::OrdType);
  FIELD_SET(*this, FIX::TimeInForce);
  FIELD_SET(*this, FIX::SelfTradePrevention);
};

} // namespace FIX42

#endif