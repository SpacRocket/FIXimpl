#include <chrono>
#include <thread>

#include "Fixtures.hpp"
#include "gtest/gtest.h"
#include "OrderTableModel.hpp"

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
  auto clOrdID = client.application.getCl0rdID();
  auto side = FIX::Side_BUY;
  auto utcTimeStamp = FIX::UtcTimeStamp();
  auto ordType = FIX::OrdType_MARKET;
  
  FIX::OrderRow Row;
  Row.aSide = side; 
  Row.aTime = utcTimeStamp; 
  Row.aOrdType = ordType;

  FIX44::NewOrderSingle message{clOrdID, side, utcTimeStamp, ordType};
  client.application.orders[clOrdID] = Row;

//Instrument component
  message.setField(FIX::Symbol("tBTCUSD"));
//QtyData
  message.setField(FIX::OrderQty(0.00000001));

  //Interpret the data received.
  FIX::Session::sendToTarget(message, client.application.getMarketSessionID());
  while(true) {
    //Check Status of the order
  };

}

TEST_F(SimpleMessages, NewOrderSingleLimitOrder) {
}

TEST_F(SimpleMessages, NewOrderSingleStopOrder) {
}

TEST_F(SimpleMessages, NewOrderSingleStopLimitOrder) {
}

TEST_F(SimpleMessages, NewOrderTrailingStopOrder) {
}