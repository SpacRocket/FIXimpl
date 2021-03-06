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
#include "quickfix/fix42/OrderCancelReject.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include <cstdlib>
#include <queue>
#include <quickfix/FixFields.h>
#include <quickfix/fix42/MessageCracker.h>
#include <random>

#include "data_structures/OrderTableModel.hpp"

namespace FIX {
/**
 * @brief Collection of callbacks for session/messages.
 */
class BfxApplication : public FIX::NullApplication, public FIX::MessageCracker {
public:
  BfxApplication();
  virtual ~BfxApplication() {}
  std::optional<FIX::SessionID> getSessionID() { return currSessionID; }
  FIX::SessionSettings settings;

  std::vector<FIX::ClOrdID> pendingOrders;
  std::vector<FIX42::ExecutionReport> executionReports;
  std::vector<FIX42::OrderCancelReject> orderCancelRejects;

public:
  FIX::ClOrdID getCl0rdID();

  std::optional<FIX::OrderID> sendNewOrderSingleLimit(
      const FIX::Symbol &symbol, const FIX::Side &side, const FIX::Price &price,
      const FIX::OrderQty &orderQty, const FIX::TimeInForce &timeInForce);
  std::optional<FIX::OrderID>
  sendNewOrderSingleMarket(const FIX::Symbol &symbol, const FIX::Side &side,
                           const FIX::OrderQty &orderQty);
  std::optional<FIX::OrderID>
  sendNewOrderSingleStopLimit(const FIX::Symbol &symbol, const FIX::Side &side,
                              const FIX::OrderQty &orderQty,
                              const FIX::Price &price,
                              const FIX::StopPx &stopPx);
  std::optional<FIX::ClOrdID>
  sendOrderStatusRequest(const FIX::OrderID &orderID,
                         const FIX::Symbol &symbol);

  void sendOrderCancelRequest(const FIX::OrigClOrdID &origClOrdID,
                              const FIX::Symbol &symbol, const FIX::Text &text);

  std::optional<FIX42::ExecutionReport>
  getExecutionReport(const float timeout, const FIX::ClOrdID aClOrdID);
  std::optional<FIX42::ExecutionReport>
  getExecutionReport(const float timeout, const FIX::OrderID aOrderID);
  std::optional<FIX42::OrderCancelReject>
  getOrderCancelReject(const float timeout, const FIX::ClOrdID aClOrdID);

protected:
  std::optional<FIX::SessionID> currSessionID;

  bool checkIfOrderIsPending(const float timeout, const FIX::ClOrdID &aClOrdID);

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

  virtual void onMessage(const FIX42::ExecutionReport &message,
                         const FIX::SessionID &) override;
  virtual void onMessage(const FIX42::OrderCancelReject &message,
                         const FIX::SessionID &) override;

private:
  Poco::UUIDGenerator uuidGenerator;
  float timeoutExecutionReport = 5.0f;
};

} // namespace FIX