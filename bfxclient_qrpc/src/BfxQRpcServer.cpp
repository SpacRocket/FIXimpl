#include "messaging.grpc.pb.h"
#include <BfxQRpcServer.hpp>
#include <google/protobuf/message.h>

Status
MessageSenderImpl::SendNewOrderSingleLimit(ServerContext *context,
                                           const OrderSingleLimit *request,
                                           TradingOrderResponse *response) {

  auto OrderSingleResponse = client.application.sendNewOrderSingleLimit(
      request->symbol(), request->side(), request->price(), request->orderqty(),
      request->timeinforce());

  if (OrderSingleResponse.has_value()) {
    response->set_orderid(OrderSingleResponse.value().getString());
  } else {
    response->set_orderid("");
  }

  return Status::OK;
}
grpc::Status
MessageSenderImpl::SendNewOrderSingleMarket(ServerContext *context,
                                            const OrderSingleMarket *request,
                                            TradingOrderResponse *response) {

  auto OrderSingleResponse = client.application.sendNewOrderSingleMarket(
      request->symbol(), request->side(), request->orderqty());

  if (OrderSingleResponse.has_value()) {
    response->set_orderid(OrderSingleResponse.value().getString());
  } else {
    response->set_orderid("");
  }

  return Status::OK;
}
grpc::Status MessageSenderImpl::SendNewOrderSingleStopLimit(
    ServerContext *context, const OrderSingleStopLimit *request,
    TradingOrderResponse *response) {

  auto OrderSingleResponse = client.application.sendNewOrderSingleStopLimit(
      request->symbol(), request->side(), request->orderqty(), request->price(),
      request->stoppx());

  if (OrderSingleResponse.has_value()) {
    response->set_orderid(OrderSingleResponse.value().getString());
  } else {
    response->set_orderid("");
  }

  return Status::OK;
}
grpc::Status
MessageSenderImpl::SendOrderStatusRequest(ServerContext *context,
                                          const OrderStatusRequest *request,
                                          InfoResponse *response) {
  auto OrderSingleResponse = client.application.sendOrderStatusRequest(
      request->orderid(), request->symbol());

  if (OrderSingleResponse.has_value()) {
    response->set_orderid(OrderSingleResponse.value().getString());
  } else {
    response->set_orderid("");
  }

  return Status::OK;
}
grpc::Status
MessageSenderImpl::SendOrderCancelRequest(ServerContext *context,
                                          const OrderCancelRequest *request,
                                          Empty *response) {

  client.application.sendOrderCancelRequest(request->origclordid(),
                                            request->symbol(), request->text());

  return Status::OK;
}
