#pragma once
#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Mutex.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/Dictionary.h"
#include "quickfix/Session.h"
#include "quickfix/SessionState.h"
#include "quickfix/SessionFactory.h"
#include "quickfix/FieldTypes.h"

#include "quickfix/fix44/Logon.h"
#include "quickfix/fix44/NewOrderSingle.h"
#include "quickfix/fix44/OrderCancelRequest.h"
#include "quickfix/fix44/OrderCancelReplaceRequest.h"
#include "quickfix/fix44/PositionReport.h"
#include "quickfix/fix44/AllocationReport.h"
#include "quickfix/fix44/ExecutionReport.h"

#include <cstdlib>

#include <queue>
#include <random>
#include "date/tz.h"
#include "date/date.h"

#include "Misc.hpp"
#include "data_structures/OrderTableModel.hpp"

namespace FIX {
/**
 * @brief Collection of callbacks for session/messages.
 */
class BfxApplication : public FIX::NullApplication, 
                       public FIX::MessageCracker
{
public:
  BfxApplication();
  virtual ~BfxApplication(){}

  std::optional<FIX::SessionID> getOrderSessionID() {return orderSessionID;}
  std::optional<FIX::SessionID> getMarketSessionID() {return marketSessionID;}
  FIX::SessionSettings settings;

  FIX::OrderTableModel orders;
  
//helpers
public:
  FIX::TransactTime getCurrentTransactTime();
  FIX::ClOrdID getCl0rdID();

  std::mt19937 gen;
  std::uniform_int_distribution<int> intDist;

protected:
  std::optional<FIX::SessionID> orderSessionID;
  std::optional<FIX::SessionID> marketSessionID;

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

  void onMessage( const FIX40::ExecutionReport&, const FIX::SessionID& ) override;
};

} //End of FIX namespace