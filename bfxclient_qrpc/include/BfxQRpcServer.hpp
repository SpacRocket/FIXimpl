#pragma once
#include <BfxApplicationQRpc.hpp>
#include <BfxClientAbstract.hpp>
#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <messaging.grpc.pb.h>
#include <messaging.pb.h>
#include <string>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

class MessageSenderImpl final : public MessageSender::Service {
public:
  virtual Status
  SendNewOrderSingleLimit(ServerContext *context,
                          const OrderSingleLimit *request,
                          TradingOrderResponse *response) override;
  virtual Status
  SendNewOrderSingleMarket(ServerContext *context,
                           const OrderSingleMarket *request,
                           TradingOrderResponse *response) override;
  virtual Status
  SendNewOrderSingleStopLimit(ServerContext *context,
                              const OrderSingleStopLimit *request,
                              TradingOrderResponse *response) override;
  virtual Status SendOrderStatusRequest(ServerContext *context,
                                        const OrderStatusRequest *request,
                                        InfoResponse *response) override;
  virtual Status SendOrderCancelRequest(ServerContext *context,
                                        const OrderCancelRequest *request,
                                        Empty *response) override;

private:
  void PrepareClient();
  FIX::BfxClient<FIX::BfxApplicationQRpc> client;
};

inline void RunServer() {
  std::string server_address("localhost:50051");
  MessageSenderImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}