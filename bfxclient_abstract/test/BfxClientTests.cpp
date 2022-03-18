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

TEST_F(MessagingTest, NewOrderSingleMarket) {
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
  // Order is in pending order vector.
  while (std::find(pendingOrderRef.begin(), pendingOrderRef.end(), aClOrdID) ==
         pendingOrderRef.end()) {
    if (stopwatch.elapsedSeconds() > 10) {
      ASSERT_TRUE(false);
    }
  }
  ASSERT_TRUE(true);
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
  FIX42::NewOrderSingle order;

  FIX::Symbol aSymbol = FIX::Symbol("BTC-USD");
  FIX::Side aSide = FIX::Side(FIX::Side_BUY);

  FIX::ClOrdID aClOrdID(client.application.getCl0rdID());
  client.application.pendingOrders.push_back(aClOrdID);

  auto pendingOrderRef = client.application.pendingOrders;

  order.set(aClOrdID);
  order.set(aSymbol);
  order.set(aSide);
  order.set(FIX::Price(200000.3));
  order.set(FIX::OrderQty(0.00003));
  order.set(FIX::OrdType('2'));
  order.set(FIX::TimeInForce('1'));

  FIX::Session::sendToTarget(order, client.application.getSessionID().value());

  Poco::Stopwatch stopwatch;
  stopwatch.start();
  // Order is in pending order vector.
  // TODO: Make this a method check if order received a execution report.
  while (std::find(pendingOrderRef.begin(), pendingOrderRef.end(), aClOrdID) ==
         pendingOrderRef.end()) {
    if (stopwatch.elapsedSeconds() > 10) {
      ASSERT_TRUE(false) << "Assert failed at confirmation of NewOrderSingle";
    }
  }

  // Find an order with following symbol
  aClOrdID = client.application.getCl0rdID();
  FIX42::OrderStatusRequest aOrderStatusRequest(aClOrdID, aSymbol, aSide);
  pendingOrderRef.push_back(aClOrdID);
  FIX::Session::sendToTarget(aOrderStatusRequest,
                             client.application.getSessionID().value());

  stopwatch.restart();
  while (std::find(pendingOrderRef.begin(), pendingOrderRef.end(), aClOrdID) ==
         pendingOrderRef.end()) {
    if (stopwatch.elapsedSeconds() > 10) {
      ASSERT_TRUE(false) << "Assert failed at searching of execution report";
    }
  }

  // Wait for execution report.
  stopwatch.restart();
  stopwatch.start();
  std::optional<FIX42::ExecutionReport> Status;
  auto &executionReportsRef = client.application.executionReports;

  // Wait for a execution report, no longer than x time.
  while (stopwatch.elapsedSeconds() < 10) {
    auto it = find_if(executionReportsRef.begin(), executionReportsRef.end(),
                      [&aClOrdID](const FIX42::ExecutionReport &obj) {
                        FIX::ClOrdID objClOrdID;
                        obj.get(objClOrdID);
                        return objClOrdID == aClOrdID;
                      });
    if (it != executionReportsRef.end()) {
      Status = *it;
      break;
    }
  }
  ASSERT_TRUE(Status.has_value()) << "Status of an order is not available.";
}

#pragma endregion SimpleMessages