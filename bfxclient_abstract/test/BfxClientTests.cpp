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
  order.set(FIX::Symbol("BTC-USD"));
  order.set(FIX::Side(FIX::Side_BUY));
  order.set(FIX::OrderQty(0.0005));
  order.set(FIX::OrdType('4'));
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

TEST_F(MessagingTest, NewOrderSingleStopLimitShort) {
  auto result{client.application.sendNewOrderSingleStopLimit(
      FIX::Symbol("BTC-USD"), FIX::Side(FIX::Side_BUY), FIX::OrderQty(0.003),
      FIX::Price(30000.0f), FIX::StopPx(2000.0f))};

  ASSERT_TRUE(result.has_value());
}

TEST_F(MessagingTest, NewOrderStatusRequest) {
  Poco::Stopwatch stopwatch;

  auto &aExecReports{client.application.executionReports};

  FIX::Symbol aSymbol("BTC-USD");
  FIX::Side aSide(FIX::Side_BUY);

  auto aNewOrderSingle_OrderID{client.application.sendNewOrderSingleLimit(
      aSymbol, aSide, FIX::Price(20000), FIX::OrderQty(0.003),
      FIX::TimeInForce('1'))};
  ASSERT_TRUE(aNewOrderSingle_OrderID.has_value())
      << "NewOrderSingle_OrderID optional isn't set";

  auto aExecReport_StatusRequest{client.application.sendOrderStatusRequest(
      aNewOrderSingle_OrderID.value(), aSymbol)};
  ASSERT_TRUE(aExecReport_StatusRequest.has_value())
      << "aExecReport_statusRequest isn't set optional isn't set";
}

#pragma endregion SimpleMessages