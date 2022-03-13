#include <quickfix/FixFields.h>
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

TEST_F(MessagingTest, NewOrderSingle) {}