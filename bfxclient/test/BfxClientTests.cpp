#include <chrono>
#include <thread>

#include "Fixtures.hpp"
#include "gtest/gtest.h"

TEST(Startup, Connection) {
  FIX::BfxClient Client;
  Client.initiator->start();

  using std::chrono::duration;
  using std::chrono::high_resolution_clock;
  using std::chrono::seconds;

  auto t1{high_resolution_clock::now()};
  seconds maxWaitingTime{7};
  duration<double> dur;

  do {
    auto t2 = high_resolution_clock::now();
    dur = t2 - t1;

  } while (!Client.initiator->isLoggedOn() && dur < maxWaitingTime);
  ASSERT_TRUE(Client.initiator->isLoggedOn());
}

TEST_F(SimpleMessages, NewOrderSingleBTC) {
    
}