#include <chrono>
#include <thread>

#include "Fixtures.hpp"
#include "gtest/gtest.h"

TEST(Startup, Connection) {
  FIX::BfxClient<> client;
  client.initiator->start();

  using std::chrono::duration;
  using std::chrono::high_resolution_clock;
  using std::chrono::seconds;

  auto t1{high_resolution_clock::now()};
  seconds maxWaitingTime{7};
  duration<double> dur;

  do {
    auto t2 = high_resolution_clock::now();
    dur = t2 - t1;

  } while (!client.initiator->isLoggedOn() && dur < maxWaitingTime);
  ASSERT_TRUE(client.initiator->isLoggedOn());
}

TEST_F(SimpleMessages, NewOrderSingleMarketOrder) {
  FIX44::NewOrderSingle message;
  message.setField(FIX::OrdType(1));


  FIX::Session::sendToTarget(message, client.application.getMarketSessionID());
  //Interpret the data received.
}

TEST_F(SimpleMessages, NewOrderSingleLimitOrder) {
}

TEST_F(SimpleMessages, NewOrderSingleStopOrder) {
}

TEST_F(SimpleMessages, NewOrderSingleStopLimitOrder) {
}

TEST_F(SimpleMessages, NewOrderTrailingStopOrder) {
}