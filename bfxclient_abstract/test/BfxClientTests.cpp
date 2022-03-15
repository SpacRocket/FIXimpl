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
  order.set(aClOrdID);

  order.set(FIX::Symbol("BTC-EUR"));
  order.set(FIX::Side(FIX::Side_BUY));
  order.set(FIX::Price(2000.0));
  order.set(FIX::OrderQty(0.001));
  order.set(FIX::OrdType('2'));
  order.set(FIX::TimeInForce('1'));

  FIX::Session::sendToTarget(order, client.application.getSessionID().value());

  while (1) {
  }
}

#pragma endregion SimpleMessages