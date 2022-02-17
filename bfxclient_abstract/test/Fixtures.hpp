#pragma once
#include <chrono>
#include <thread>

#include "BfxClientAbstract.hpp"
#include "Fixtures.hpp"
#include "gtest/gtest.h"

class MessagingTest : public ::testing::Test {
 protected:

  void SetUp() override {
    client = FIX::BfxClient<>(FIX::SSLMode::SSL);
    if(client.has_value() == false) ASSERT_TRUE(false);
    
    client.value().initiator->start();

    using std::chrono::duration;
    using std::chrono::high_resolution_clock;
    using std::chrono::seconds;

    auto t1{high_resolution_clock::now()};
    seconds maxWaitingTime{7};
    duration<double> dur;

    do {
      auto t2 = high_resolution_clock::now();
      dur = t2 - t1;

    } while (!client.value().initiator->isLoggedOn() && dur < maxWaitingTime);
    EXPECT_TRUE(client.value().initiator->isLoggedOn()) << "Reason: Timeout";
  }
  // void TearDown() override {}
public:
  std::optional<FIX::BfxClient<>> client;
};

class SimpleMessages : public MessagingTest{};