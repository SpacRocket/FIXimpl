#pragma once
#include <chrono>
#include <thread>

#include "BfxClientAbstract.hpp"
#include "Fixtures.hpp"
#include "gtest/gtest.h"

class MessagingTest : public ::testing::Test {
  public:
  ~MessagingTest() throw() {}
 protected:
  void SetUp() override {
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
    EXPECT_TRUE(client.initiator->isLoggedOn()) << "Reason: Timeout";
  }
  // void TearDown() override {}
public:
  FIX::BfxClient<> client;
};

class SimpleMessages : public MessagingTest{};