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

#include <cstdlib>

#include <queue>
#include <random>
#include "date/tz.h"
#include "date/date.h"

namespace FIX {
/**
 * @brief Collection of callbacks for session/messages.
 */
class BfxApplication : public FIX::NullApplication, 
                       public FIX::MessageCracker
{
public:
  BfxApplication() {};
  virtual ~BfxApplication();

  FIX::SessionID getOrderSessionID();

  void BsMethod();

  FIX::SessionSettings settings;

//helpers
  FIX::TransactTime getCurrentTransactTime();

protected:
  FIX::SessionID orderSessionID;

  //CL0RDID generation.
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> intDist{0, INT32_MAX};

private:
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
  EXCEPT ( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType ) override;

//OnMessage

//MarketTypes

//queryHeaderVersionActionConfirm

//Fields like SenderComp, etc.
};

}

/* ToDo: Incorporate id to the orderid
*/