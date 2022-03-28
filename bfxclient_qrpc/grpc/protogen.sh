#!/bin/bash

SOURCEDIR=.
DESTINATION=./gen

protoc -I . --cpp_out=$DESTINATION ./messaging.proto
protoc -I . --grpc_out=$DESTINATION --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./messaging.proto