#include "BfxApplication.hpp"

#include "quickfix/Session.h"

#ifdef HAVE_POSTGRESQL
#include "quickfix/PostgreSQLConnection.h"
#include "quickfix/PostgreSQLLog.h"
#include "quickfix/PostgreSQLStore.h"
#endif


namespace FIX {

BfxApplication::BfxApplication(){
  //Boilerplate
  gen = std::mt19937(time(nullptr));
  intDist = std::uniform_int_distribution<int>(0, INT_MAX);
}

void BfxApplication::onCreate(const SessionID &sessionID) {
  std::cout << YELLOW << "Session created - Session: " << RESET << sessionID.toString() << "\n";

  if(sessionID.getTargetCompID() == FIX::TargetCompID("BFXFIX") && 
     sessionID.getSenderCompID() == FIX::SenderCompID("EXORG_ORD"))
  {
  orderSessionID = sessionID;
  }
  else if(sessionID.getTargetCompID() == FIX::TargetCompID("BFXFIX") && 
     sessionID.getSenderCompID() == FIX::SenderCompID("EXORG_ORD"))
  {
  marketSessionID = sessionID;
  }

};

void BfxApplication::onLogon(const SessionID &sessionID) {
  std::cout << GREEN << "Logon - Session: " << RESET << sessionID.toString() << "\n";
}

void BfxApplication::onLogout(const SessionID &sessionID) {
  std::cout << RED << "Logout - Session: " << RESET << sessionID.toString() << "\n";
}

void BfxApplication::toAdmin(Message &message, const SessionID &sessionID) {
  FIX::MsgType msgType;
  message.getHeader().getField(msgType);

  if (msgType == MsgType_Logon) {
    const auto &defaultDict = settings.get();
    message.setField(20000, defaultDict.getString("APIKEY"));
    message.setField(20001, defaultDict.getString("APISECRET"));
    message.setField(20002, defaultDict.getString("BFXUSER"));
  }
}

void BfxApplication::toApp(Message &message, const SessionID &sessionID)
    EXCEPT(DoNotSend) {}

void BfxApplication::fromAdmin(const Message &message,
                               const SessionID &sessionID)
    EXCEPT(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, RejectLogon) {
}

void BfxApplication::fromApp(const Message &message, const SessionID &sessionID)
    EXCEPT(FieldNotFound, IncorrectDataFormat, IncorrectTagValue,
           UnsupportedMessageType) {
  crack(message, sessionID);
}

void BfxApplication::onMessage( const FIX44::ExecutionReport& argMsg, const FIX::SessionID& argSessionID)
{ 
  FIX::ClOrdID clOrdID;

  //OrdStatus
  FIX::OrdStatus ordStatus;
  try{
    argMsg.get(ordStatus);
    argMsg.get(clOrdID);
    if(ordStatus.getValue() == FIX::OrdStatus_REJECTED || ordStatus.getValue() == FIX::OrdStatus_NEW){
      orders[clOrdID].aOrdStatus = ordStatus.getValue();
    }
  }catch( ... ){}
  
}

// Helper methods
FIX::TransactTime BfxApplication::getCurrentTransactTime() {
  using namespace std::chrono;
  using date::operator<<;

  auto tp = system_clock::now();
  std::stringstream utcTimeStamp;
  utcTimeStamp << tp;
  std::stringstream currentTime;
  FIX::TransactTime transactTime;
  transactTime.setString(currentTime.str());

  return transactTime;
}

FIX::ClOrdID BfxApplication::getCl0rdID(){
  return FIX::ClOrdID(std::to_string(intDist(gen)));
}

}  // namespace FIX`