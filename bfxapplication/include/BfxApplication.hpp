#pragma once
#include "quickfix/Application.h"
#include "quickfix/Dictionary.h"
#include "quickfix/FieldTypes.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Mutex.h"
#include "quickfix/Session.h"
#include "quickfix/SessionFactory.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/SessionState.h"

#include "Misc.hpp"
#include "Poco/UUIDGenerator.h"
#include "date/date.h"
#include "date/tz.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/fix42/Logon.h"
#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include <cstdlib>
#include <queue>
#include <random>

#include "data_structures/OrderTableModel.hpp"

// TODO: Implement Failed and Succesful details for NewOrderSingle.

namespace FIX {
/**
 * @brief Collection of callbacks for session/messages.
 */
class BfxApplication : public FIX::NullApplication, public FIX::MessageCracker {
public:
  BfxApplication();
  virtual ~BfxApplication() {}
  std::optional<FIX::SessionID> getSessionID() { return currSessionID; }

  void interpretExecutionOrder();

  FIX::SessionSettings settings;
  FIX::OrderTableModel orders;

public:
  FIX::ClOrdID getCl0rdID();

  // Implementation of orders

protected:
  std::optional<FIX::SessionID> currSessionID;

private:
  //--- Message Handlers ---
  /// Notification of a session begin created
  virtual void onCreate(const FIX::SessionID &) override;
  /// Notification of a session successfully logging on
  virtual void onLogon(const FIX::SessionID &) override;
  /// Notification of a session logging off or disconnecting
  virtual void onLogout(const FIX::SessionID &) override;
  /// Notification of admin message being sent to target
  virtual void toAdmin(FIX::Message &, const FIX::SessionID &) override;
  /// Notification of app message being sent to target
  virtual void toApp(FIX::Message &, const FIX::SessionID &)
      EXCEPT(FIX::DoNotSend) override;
  /// Notification of admin message being received from target
  virtual void fromAdmin(const FIX::Message &, const FIX::SessionID &)
      EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat,
             FIX::IncorrectTagValue, FIX::RejectLogon) override;
  /// Notification of app message being received from target
  virtual void fromApp(const FIX::Message &, const FIX::SessionID &)
      EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat,
             FIX::IncorrectTagValue, FIX::UnsupportedMessageType) override;

  void onMessage(const FIX42::ExecutionReport &,
                 const FIX::SessionID &) override;

private:
  Poco::UUIDGenerator uuidGenerator;
};

} // namespace FIX