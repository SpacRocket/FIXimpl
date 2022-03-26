cmake_minimum_required(VERSION 3.21)
include_guard(GLOBAL)
set(CMAKE_CXX_STANDARD 20)
project(BfxClientQRpc)

find_package(Quickfix REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

add_executable(BfxClientQRpc)

target_link_libraries(BfxClientQRpc 
    PUBLIC Quickfix::Quickfix
    PUBLIC BfxApplication
    PRIVATE BfxClientAbstract
    PUBLIC gRPC::grpc++
    )

target_include_directories(BfxClientQRpc
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/grpc/gen
    PUBLIC gRPC::grpc++
    PUBLIC Quickfix::Quickfix
    PUBLIC BfxApplication
    PRIVATE BfxClientAbstract
    )

target_sources(BfxClientQRpc PRIVATE "./src/BfxApplicaitonQRpc.cpp"
                             PUBLIC "./src/BfxClientQRpc.cpp"
                             PUBLIC "./src/BfxQRpcEntryPoint.cpp"
                             PUBLIC "./src/BfxQRpcServer.cpp"
                             PUBLIC "./grpc/gen/messaging.pb.cc"
                             PUBLIC "./grpc/gen/messaging.grpc.pb.cc"
                             )