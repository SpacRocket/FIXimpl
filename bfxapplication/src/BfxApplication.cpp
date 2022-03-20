#include "BfxApplication.hpp"
#include "quickfix/Session.h"

#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/HMACEngine.h"
#include "Poco/SHA2Engine.h"
#include "Poco/StreamCopier.h"

#include <Poco/Stopwatch.h>
#include <istream>

#include <quickfix/FieldTypes.h>
#include <quickfix/FixFieldNumbers.h>
#include <quickfix/FixFields.h>
#include <quickfix/FixValues.h>
#include <quickfix/fix42/ExecutionReport.h>
#include <quickfix/fix42/NewOrderSingle.h>
#include <string_view>

namespace Poco {
class SHA256Engine : public Poco::SHA2Engine {
public:
  enum { BLOCK_SIZE = 64, DIGEST_SIZE = 32 };
  SHA256Engine() : Poco::SHA2Engine(Poco::SHA2Engine::ALGORITHM::SHA_256) {}
  virtual ~SHA256Engine() {}
};
} // namespace Poco

namespace FIX {

BfxApplication::BfxApplication() {
  // Boilerplate
}

void BfxApplication::onCreate(const SessionID &sessionID) {
  std::cout << YELLOW << "Session created - Session: " << RESET
            << sessionID.toString() << "\n";
  this->currSessionID = sessionID;
};

void BfxApplication::onLogon(const SessionID &sessionID) {
  std::cout << GREEN << "Logon - Session: " << RESET << sessionID.toString()
            << "\n";
}

void BfxApplication::onLogout(const SessionID &sessionID) {
  std::cout << RED << "Logout - Session: " << RESET << sessionID.toString()
            << "\n";
}

void BfxApplication::toAdmin(Message &message, const SessionID &sessionID) {

  FIX::MsgType msgType;
  message.getHeader().getField(msgType);

  if (msgType == MsgType_Logon) {
    const auto &defaultDict = settings.get();
    std::string password = defaultDict.getString("Passphrase");
    std::string apiSecret = defaultDict.getString("ApiSecret");

    FIX::RawData rawData;
    std::stringstream preHashStringStream;

    FIX::UtcTimeStamp now;
    message.getHeader().setField(FIX::SendingTime(now, false));

    preHashStringStream
        << message.getHeader().getField(FIX::FIELD::SendingTime) << '\x01'
        << msgType << '\x01'
        << message.getHeader().getField(FIX::FIELD::MsgSeqNum) << '\x01'
        << message.getHeader().getField(FIX::FIELD::SenderCompID) << '\x01'
        << message.getHeader().getField(FIX::FIELD::TargetCompID) << '\x01'
        << password;
    std::string preHashString{preHashStringStream.str()};

    /*
    HMAC sha256 using base64-decoded secretkey, on prehashstring
    then base encode hmac sha256 digest output
    */

    // Decode apiSecret
    std::istringstream apiSecretStream(apiSecret);
    Poco::Base64Decoder b64apiSecret(apiSecretStream);
    std::string decodedSecret;
    Poco::StreamCopier::copyToString(b64apiSecret, decodedSecret);

    // HMAC sha256
    Poco::HMACEngine<Poco::SHA256Engine> signingEngine(decodedSecret);
    signingEngine.update(preHashString);
    auto digest = signingEngine.digest();

    std::ostringstream digestBase64;
    Poco::Base64Encoder base64Encoder(digestBase64);
    base64Encoder.write(reinterpret_cast<char *>(&digest[0]), digest.size());
    base64Encoder.close();

    rawData.setString(digestBase64.str());

    // Fields set according to the table
    message.setField(FIX::EncryptMethod(0));
    message.setField(FIX::HeartBtInt(30));
    message.setField(FIX::Password(password));
    message.setField(rawData);
    std::string TestOutput{digestBase64.str()};
    message.setField(FIX::RawDataLength(TestOutput.length()));
    message.setField(8013, "Y");

    this->currSessionID = sessionID;
  }
  message.getTrailer().setField(FIX::CheckSum(message.checkSum()));
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

void BfxApplication::onMessage(const FIX42::ExecutionReport &message,
                               const FIX::SessionID &) {
  FIX::ExecType aExecType;
  message.get(aExecType);

  if (aExecType == FIX::ExecType_NEW) {
    FIX::ClOrdID aClOrdID;
    message.getField(aClOrdID);

    pendingOrders.erase(
        std::remove(pendingOrders.begin(), pendingOrders.end(), aClOrdID),
        pendingOrders.end());

    executionReports.push_back(message);
  }

  else if (aExecType == FIX::ExecType_ORDER_STATUS) {
    FIX::ClOrdID aClOrdID;
    message.getField(aClOrdID);

    pendingOrders.erase(
        std::remove(pendingOrders.begin(), pendingOrders.end(), aClOrdID),
        pendingOrders.end());

    executionReports.push_back(message);
  }
}

FIX::ClOrdID BfxApplication::getCl0rdID() {
  return FIX::ClOrdID(uuidGenerator.create().toString());
}

std::optional<FIX::OrderID> BfxApplication::sendNewOrderSingleLimit(
    const FIX::Symbol &symbol, const FIX::Side &side, const FIX::Price &price,
    const FIX::OrderQty &orderQty, const FIX::TimeInForce &timeInForce) {

  // Pending order
  FIX::ClOrdID aClOrdID(getCl0rdID());
  pendingOrders.push_back(aClOrdID);

  // Prepare message
  FIX42::NewOrderSingle order;
  order.set(aClOrdID);
  order.set(symbol);
  order.set(side);
  order.set(price);
  order.set(orderQty);
  order.set(timeInForce);
  order.set(FIX::OrdType('2'));
  FIX::Session::sendToTarget(order, getSessionID().value());

  // Order is in pending order vector.
  if (checkIfOrderIsPending(timeoutExecutionReport, aClOrdID)) {
    return {};
  }

  // Wait for execution report
  auto executionReport = getExecutionReport(timeoutExecutionReport, aClOrdID);
  if (executionReport.has_value() == false)
    return {};

  // Get orderID
  return executionReport.value().getField(FIX::FIELD::OrderID);
}

std::optional<FIX::OrderID>
BfxApplication::sendNewOrderSingleMarket(const FIX::Symbol &symbol,
                                         const FIX::Side &side,
                                         const FIX::OrderQty &orderQty) {
  // Pending order
  FIX::ClOrdID aClOrdID(getCl0rdID());
  pendingOrders.push_back(aClOrdID);

  // Prepare message
  FIX42::NewOrderSingle order;
  order.set(aClOrdID);
  order.set(symbol);
  order.set(side);
  order.set(orderQty);
  order.set(FIX::OrdType('1'));
  FIX::Session::sendToTarget(order, getSessionID().value());

  // Order is in pending order vector.
  if (checkIfOrderIsPending(timeoutExecutionReport, aClOrdID)) {
    return {};
  }
  // Wait for execution report
  auto executionReport = getExecutionReport(timeoutExecutionReport, aClOrdID);
  if (executionReport.has_value() == false)
    return {};
  // Get orderID
  return executionReport.value().getField(FIX::FIELD::OrderID);
}

std::optional<FIX42::ExecutionReport>
BfxApplication::getExecutionReport(const float timeout,
                                   const FIX::ClOrdID aClOrdID) {
  Poco::Stopwatch stopwatch;
  stopwatch.start();

  stopwatch.restart();
  while (stopwatch.elapsedSeconds() < timeoutExecutionReport) {
    auto aExecReportsCopy = executionReports;
    auto it{std::find_if(aExecReportsCopy.begin(), aExecReportsCopy.end(),
                         [aClOrdID](const FIX42::ExecutionReport &obj) -> bool {
                           return obj.getField(FIX::FIELD::ClOrdID) == aClOrdID;
                         })};
    if (it != aExecReportsCopy.end()) {
      return *it;
      break;
    }
  }
  return {};
}

bool BfxApplication::checkIfOrderIsPending(const float timeout,
                                           const FIX::ClOrdID &aClOrdID) {
  Poco::Stopwatch stopwatch;
  stopwatch.start();

  while (std::find(pendingOrders.begin(), pendingOrders.end(), aClOrdID) ==
         pendingOrders.end()) {
    if (stopwatch.elapsedSeconds() > timeout) {
      return true;
    }
  }
  return false;
}
} // namespace FIX