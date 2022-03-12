#pragma once
#include <chrono>
#include <thread>

#include "BfxClientAbstract.hpp"
#include "Fixtures.hpp"
#include "Poco/Stopwatch.h"
#include "gtest/gtest.h"

class MessagingTest : public ::testing::Test {
  public:
  ~MessagingTest() throw() {}
 protected:
  void SetUp() override {
      client.initiator->start();
      Poco::Stopwatch stopwatch;
      stopwatch.start();
      do{
      } while (!client.initiator->isLoggedOn() && stopwatch.elapsedSeconds() < 3);
      ASSERT_TRUE(client.initiator->isLoggedOn());
  }
public:
    FIX::BfxClient<> client{};
};

class SimpleMessages : public MessagingTest{};