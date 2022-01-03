#include "gtest/gtest.h"
#include "BfxClient.hpp"
#include <chrono>
#include <thread>

TEST(AuthenticaitonTest, Logon){
    FIX::BfxClient Client;
    Client.initiator->start();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ASSERT_TRUE(Client.initiator->isLoggedOn());
}