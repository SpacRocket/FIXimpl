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
  }
  // void TearDown() override {}
public:
};

class SimpleMessages : public MessagingTest{};