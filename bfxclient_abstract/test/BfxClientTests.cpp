#include <Poco/Stopwatch.h>
#include <quickfix/FixFieldNumbers.h>
#include <quickfix/FixFields.h>
#include <quickfix/FixValues.h>
#include <quickfix/fix42/ExecutionReport.h>
#include <quickfix/fix42/MessageCracker.h>
#include <quickfix/fix42/NewOrderSingle.h>
#include <quickfix/fix42/OrderStatusRequest.h>

#include <chrono>
#include <thread>

#include "Fixtures.hpp"
#include "gtest/gtest.h"

#pragma region SetupCheck

TEST(Startup, Connection) {
  FIX::BfxClient<> client{};
  client.initiator->start();

  Poco::Stopwatch stopwatch;
  stopwatch.start();
  do {
  } while (!client.initiator->isLoggedOn() && stopwatch.elapsedSeconds() < 3);

  ASSERT_TRUE(client.initiator->isLoggedOn());
}

TEST_F(MessagingTest, TestConnectionInFixture) {}

#pragma endregion SetupCheck

#pragma region SimpleMessages

TEST_F(MessagingTest, NewOrderSingleLimit) {
  FIX42::NewOrderSingle order;

  FIX::ClOrdID aClOrdID(client.application.getCl0rdID());
  client.application.pendingOrders.push_back(aClOrdID);

  auto pendingOrderRef = client.application.pendingOrders;

  order.set(aClOrdID);
  order.set(FIX::Symbol("BTC-USD"));
  order.set(FIX::Side(FIX::Side_BUY));
  order.set(FIX::Price(500000.3));
  order.set(FIX::OrderQty(0.0003));
  order.set(FIX::OrdType('2'));
  order.set(FIX::TimeInForce('1'));

  FIX::Session::sendToTarget(order, client.application.getSessionID().value());

  Poco::Stopwatch stopwatch;
  stopwatch.start();
  // Order is in pending order vector.
  while (std::find(pendingOrderRef.begin(), pendingOrderRef.end(), aClOrdID) ==
         pendingOrderRef.end()) {
    if (stopwatch.elapsedSeconds() > 10) {
      ASSERT_TRUE(false);
    }
  }
  ASSERT_TRUE(true);
}

TEST_F(MessagingTest, NewOrderSingleLimitShort) {
  auto result{client.application.sendNewOrderSingleLimit(
      FIX::Symbol("BTC-USD"), FIX::Side(FIX::Side_BUY), FIX::Price(50000.3),
      FIX::OrderQty(0.0003), FIX::TimeInForce('1'))};

  ASSERT_TRUE(result.has_value());
}

TEST_F(MessagingTest, NewOrderSingleMarket) {
  // Doing a new order single.
  FIX42::NewOrderSingle order;

  FIX::ClOrdID aClOrdID(client.application.getCl0rdID());
  client.application.pendingOrders.push_back(aClOrdID);

  auto pendingOrderRef = client.application.pendingOrders;

  order.set(aClOrdID);
  order.set(FIX::Symbol("BTC-USD"));
  order.set(FIX::Side(FIX::Side_BUY));
  order.set(FIX::OrderQty(0.00003));
  order.set(FIX::OrdType('1'));

  FIX::Session::sendToTarget(order, client.application.getSessionID().value());

  Poco::Stopwatch stopwatch;
  stopwatch.start();

  while (std::find(pendingOrderRef.begin(), pendingOrderRef.end(), aClOrdID) ==
         pendingOrderRef.end()) {
    if (stopwatch.elapsedSeconds() > 10) {
      ASSERT_TRUE(false);
    }
  }
  ASSERT_TRUE(true);
}

TEST_F(MessagingTest, NerOrderSingleMarketShort) {
  auto result{client.application.sendNewOrderSingleMarket(
      FIX::Symbol("BTC-USD"), FIX::Side(FIX::Side_BUY), FIX::OrderQty(0.0003))};

  ASSERT_TRUE(result.has_value());
}

TEST_F(MessagingTest, NewOrderSingleStopLimit) {
  FIX42::NewOrderSingle order;

  FIX::ClOrdID aClOrdID(client.application.getCl0rdID());
  client.application.pendingOrders.push_back(aClOrdID);

  auto pendingOrderRef = client.application.pendingOrders;

  order.set(aClOrdID);
  order.set(FIX::Symbol("BTC-BTC"));
  order.set(FIX::Side(FIX::Side_BUY));
  order.set(FIX::OrderQty(0.0005));
  order.set(FIX::OrdType('1'));
  order.set(FIX::StopPx(2000.0));

  FIX::Session::sendToTarget(order, client.application.getSessionID().value());

  Poco::Stopwatch stopwatch;
  stopwatch.start();
  // Order is in pending order vector.
  while (std::find(pendingOrderRef.begin(), pendingOrderRef.end(), aClOrdID) ==
         pendingOrderRef.end()) {
    if (stopwatch.elapsedSeconds() > 10) {
      ASSERT_TRUE(false);
    }
  }
  ASSERT_TRUE(true);
}

TEST_F(MessagingTest, NewOrderStatusRequest) {
  Poco::Stopwatch stopwatch;

  auto &aExecReports{client.application.executionReports};
  // 1. Doing a new order single.
  FIX42::NewOrderSingle order;
  FIX::ClOrdID aClOrdNewOrderSingle(client.application.getCl0rdID());
  client.application.pendingOrders.push_back(aClOrdNewOrderSingle);

  auto pendingOrderRef = client.application.pendingOrders;

  FIX::Symbol aSymbol{"BTC-USD"};
  FIX::Side aSide{FIX::Side_BUY};

  order.set(aClOrdNewOrderSingle);
  order.set(aSymbol);
  order.set(aSide);
  order.set(FIX::OrderQty(0.00003));
  order.set(FIX::OrdType('1'));

  FIX::Session::sendToTarget(order, client.application.getSessionID().value());

  stopwatch.start();

  while (std::find(pendingOrderRef.begin(), pendingOrderRef.end(),
                   aClOrdNewOrderSingle) == pendingOrderRef.end()) {
    if (stopwatch.elapsedSeconds() > 10) {
      ASSERT_TRUE(false);
    }
  }

  // 2. Execution report based on ClOrdID of NewOrderSingle.
  FIX42::ExecutionReport aExecReportNewOrderSingle{};
  stopwatch.restart();
  while (stopwatch.elapsedSeconds() < 10) {
    auto aExecReportsCopy = aExecReports; // This could be improved.
    auto it{std::find_if(
        aExecReportsCopy.begin(), aExecReportsCopy.end(),
        [aClOrdNewOrderSingle](const FIX42::ExecutionReport &obj) -> bool {
          return obj.getField(FIX::FIELD::ClOrdID) == aClOrdNewOrderSingle;
        })};
    if (it != aExecReportsCopy.end()) {
      aExecReportNewOrderSingle = *it;
      break;
    }
  }

  // 2.2. Check if there is execution report.
  ASSERT_TRUE(aExecReportNewOrderSingle.isSetField(FIX::FIELD::ClOrdID))
      << "Execution Report New Order Single is empty. Might mean that there is "
         "no matching ExecReportNewOrderSingle."
      << std::endl;

  // 3. f(ExecutionReportFromNewOrderSingle) -> OrderID.
  FIX::OrderID aNewOrderSingleID{};
  aExecReportNewOrderSingle.get(aNewOrderSingleID);

  // 4. Send Order Status Request (H)
  FIX::ClOrdID aOrderStatusReq_ClOrdID{client.application.getCl0rdID()};
  FIX42::OrderStatusRequest aOrderStatusRequest(aOrderStatusReq_ClOrdID,
                                                aSymbol, aSide);

  pendingOrderRef.push_back(aOrderStatusReq_ClOrdID);
  FIX::Session::sendToTarget(aOrderStatusRequest,
                             client.application.getSessionID().value());

  // 4.2 Wait for a matching execution report.
  stopwatch.restart();
  while (std::find(pendingOrderRef.begin(), pendingOrderRef.end(),
                   aOrderStatusReq_ClOrdID) == pendingOrderRef.end()) {
    if (stopwatch.elapsedSeconds() > 10) {
      ASSERT_TRUE(false)
          << "no matching execution report for NewOrderSingleStatus"
          << std::endl;
    }
  }

  // 4.3 Read execution report, matching executionReport is defined by ClOrdID
  // the same as SendingOrderStatusID.
  // TODO: Try to create a new method out of it
  FIX42::ExecutionReport aStatusExecutionReport;

  stopwatch.restart();
  while (stopwatch.elapsedSeconds() < 10) {
    auto aExecReportsCopy = aExecReports;
    auto it{std::find_if(
        aExecReportsCopy.begin(), aExecReportsCopy.end(),
        [aOrderStatusReq_ClOrdID](const FIX42::ExecutionReport &obj) -> bool {
          return obj.getField(FIX::FIELD::ClOrdID) == aOrderStatusReq_ClOrdID;
        })};
    if (it != aExecReportsCopy.end()) {
      aStatusExecutionReport = *it;
      break;
    }
  }
  ASSERT_TRUE(aStatusExecutionReport.isSetField(FIX::FIELD::ClOrdID))
      << "aStatusExecutionReport isn't filed, the problem might lie with: "
         "reading of the available messages or lack of response from the server"
      << std::endl;
  //---
}

#pragma endregion SimpleMessages