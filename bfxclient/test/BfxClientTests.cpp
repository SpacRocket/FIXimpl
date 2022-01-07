#include "gtest/gtest.h"
#include "BfxClient.hpp"
#include <chrono>
#include <thread>

TEST(AuthenticaitonTest, Logon){

}

TEST(OrderTest, Order){
    FIX::BfxClient cl;
    cl.initiator->start();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    if(cl.initiator->isLoggedOn()){
        
        FIX42::NewOrderSingle order{
            cl.application.getCl0rdID(),
            FIX::HandlInst(2),
            FIX::Symbol("btcusd"),
            FIX::Side(1),
            cl.application.getCurrentTransactTime(),
            FIX::OrdType((int)FIX::OrderType::Limit),
        };

        order.setField(FIX::OrderQty(0.0005));

        FIX::Session::sendToTarget(order, cl.application.getOrderSessionID());

    }
}