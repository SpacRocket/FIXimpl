#include "gtest/gtest.h"
#include "BfxClient.hpp"
#include "Fixtures.hpp"
#include <chrono>
#include <thread>

FIX::BfxClient Client;

TEST(Startup, Connection){
    FIX::BfxClient Client;
    Client.initiator->start();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ASSERT_EQ(true, Client.initiator->isLoggedOn());
    EXPECT_TRUE(false) << "Tip: The gateway might be not running.";
}

TEST(SendingMessages, NewOrderSingleBTC){
    FIX::BfxClient Client;
    Client.initiator->start();
}