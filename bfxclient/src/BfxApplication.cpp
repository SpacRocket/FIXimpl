#include "BfxApplication.hpp"

#include "quickfix/Session.h"

namespace FIX {

// ToDo: Write proper destructor
BfxApplication::~BfxApplication() {}

BfxApplication::BfxApplication() : gen(rd) {}

void BfxApplication::onCreate(const SessionID &sessionID) {
  std::cout << "Session created - Session: " << sessionID.toString() << "\n";
  orderSessionID = sessionID;
};

void BfxApplication::onLogon(const SessionID &sessionID) {
  std::cout << "Logon - Session: " << sessionID.toString() << "\n";
}

void BfxApplication::onLogout(const SessionID &sessionID) {
  std::cout << "Logout - Session: " << sessionID.toString() << "\n";
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

SessionID BfxApplication::getOrderSessionID() { return orderSessionID; }

void BfxApplication::BsMethod() {
  /*
      const FIX::ClOrdID& aClOrdID,
      const FIX::HandlInst& aHandlInst,
      const FIX::Symbol& aSymbol,
      const FIX::Side& aSide,
      const FIX::TransactTime& aTransactTime,
      const FIX::OrdType& aOrdType )
  */
  // ToDo: Incorporate date to the ClOrdID

  FIX42::NewOrderSingle order{FIX::ClOrdID(std::to_string(intDist(gen))),
                              FIX::HandlInst(2),
                              FIX::Symbol("btcusd"),
                              FIX::Side(1),
                              getCurrentTransactTime(),
                              FIX::OrdType()};

  // Session::sendToTarget(, getOrderSessionID());
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

}  // namespace FIX