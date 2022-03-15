#ifndef FIX42_NEWORDERBATCHREJECT_H
#define FIX42_NEWORDERBATCHREJECT_H

#include "custom_messages/FixCoinbaseFields.h"
#include "quickfix/fix42/Message.h"
#include <quickfix/FixFields.h>

namespace FIX42 {

class NewOrderBatchReject : public Message {
public:
  NewOrderBatchReject() : Message(MsgType()) {}
  NewOrderBatchReject(const FIX::Message &m) : Message(m) {}
  NewOrderBatchReject(const Message &m) : Message(m) {}
  NewOrderBatchReject(const NewOrderBatchReject &m) : Message(m) {}
  static FIX::MsgType MsgType() { return FIX::MsgType("U7"); }

  FIELD_SET(*this, FIX::BatchID);
  FIELD_SET(*this, FIX::Text);
};

} // namespace FIX42
#endif