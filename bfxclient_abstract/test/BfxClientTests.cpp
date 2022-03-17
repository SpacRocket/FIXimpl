#include <quickfix/FixFields.h>
#include <quickfix/FixValues.h>
#include <quickfix/fix42/NewOrderSingle.h>

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
  order.set(FIX::Side(FIX::Side_SELL));
  order.set(FIX::Price(500000.3));
  order.set(FIX::OrderQty(0.000000003));
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
  while (true)
    ASSERT_TRUE(true);
}

TEST_F(MessagingTest, NewOrderSingleMarket) {
  FIX42::NewOrderSingle order;

  FIX::ClOrdID aClOrdID(client.application.getCl0rdID());
  client.application.pendingOrders.push_back(aClOrdID);

  auto pendingOrderRef = client.application.pendingOrders;

  order.set(aClOrdID);
  order.set(FIX::Symbol("BTC-USD"));
  order.set(FIX::Side(FIX::Side_SELL));
  order.set(FIX::OrderQty(0.000003));
  order.set(FIX::OrdType('1'));

  FIX::Session::sendToTarget(order, client.application.getSessionID().value());
  while (true) {
  }

  Poco::Stopwatch stopwatch;
  stopwatch.start();
  // Order is in pending order vector.
  while (std::find(pendingOrderRef.begin(), pendingOrderRef.end(), aClOrdID) ==
         pendingOrderRef.end()) {
    if (stopwatch.elapsedSeconds() > 10) {
      ASSERT_TRUE(false);
    }
  }
  while (true) {
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
  order.set(FIX::OrderQty(0.000005));
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

#pragma endregion SimpleMessages