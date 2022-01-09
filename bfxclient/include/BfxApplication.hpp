#pragma once
#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Mutex.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/Dictionary.h"
#include "quickfix/Session.h"
#include "quickfix/SessionState.h"
#include "quickfix/SessionFactory.h"

#include "quickfix/fix42/Logon.h"
#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"

#include <cstdlib>

#include <queue>
#include <random>
#include "date/tz.h"
#include "date/date.h"

#include "OrderTypes.hpp"
#include "Misc.hpp"

namespace FIX {
/**
 * @brief Collection of callbacks for session/messages.
 */
class BfxApplication : public FIX::NullApplication, 
                       public FIX::MessageCracker
{
public:
  BfxApplication();
  virtual ~BfxApplication();

  FIX::SessionID getOrderSessionID();

  void BsMethod();

  FIX::SessionSettings settings;

//cli
public:
  void run();
private:
  char queryAction();
  bool queryConfirm( const std::string& query );
  void queryHeader( FIX::Header& header );

  void queryEnterOrder();
  FIX42::NewOrderSingle queryNewOrderSingle42();
  FIX42::OrderCancelRequest queryOrderCancelRequest42();
  FIX42::OrderCancelReplaceRequest queryCancelReplaceRequest42();

  void queryCancelOrder();
  void queryReplaceOrder();
  void queryMarketDataRequest();

  FIX::SenderCompID querySenderCompID();
  FIX::TargetCompID queryTargetCompID();
  FIX::TargetSubID queryTargetSubID();
  FIX::ClOrdID queryClOrdID();
  FIX::OrigClOrdID queryOrigClOrdID();
  FIX::Symbol querySymbol();
  FIX::Side querySide();
  FIX::OrderQty queryOrderQty();
  FIX::OrdType queryOrdType();
  FIX::Price queryPrice();
  FIX::StopPx queryStopPx();
  FIX::TimeInForce queryTimeInForce();

//helpers
public:
  static FIX::TransactTime getCurrentTransactTime();
  static FIX::ClOrdID getCl0rdID();


  inline static std::optional<std::mt19937> gen;
  inline static std::optional<std::uniform_int_distribution<int>> intDist;

protected:
  FIX::SessionID orderSessionID;

private:
//--- Message Handlers ---
  /// Notification of a session begin created
  virtual void onCreate( const FIX::SessionID& ) override;
  /// Notification of a session successfully logging on
  virtual void onLogon( const FIX::SessionID& ) override;
  /// Notification of a session logging off or disconnecting
  virtual void onLogout( const FIX::SessionID& ) override;
  /// Notification of admin message being sent to target
  virtual void toAdmin( FIX::Message&, const FIX::SessionID& ) override;
  /// Notification of app message being sent to target
  virtual void toApp( FIX::Message&, const FIX::SessionID& )
  EXCEPT ( FIX::DoNotSend ) override;
  /// Notification of admin message being received from target
  virtual void fromAdmin( const FIX::Message&, const FIX::SessionID& )
  EXCEPT ( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) override;
  /// Notification of app message being received from target
  virtual void fromApp( const FIX::Message&, const FIX::SessionID& )
  EXCEPT ( FIX::FieldNotFound, FIX::IncorrectDataFormat, 
           FIX::IncorrectTagValue, FIX::UnsupportedMessageType ) override;  

  void onMessage( const FIX42::ExecutionReport&, const FIX::SessionID& ){}
  void onMessage( const FIX42::OrderCancelReject&, const FIX::SessionID& ){}
  void onMessage( const FIX42::Reject&, const FIX::SessionID& ){}
  void onMessage( const FIX42::Heartbeat&, const FIX::SessionID& ){}
  void onMessage( const FIX42::AllocationInstructionAck&, const FIX::SessionID& ){}
  void onMessage( const FIX42::Allocation&, const FIX::SessionID& ){}

};

}