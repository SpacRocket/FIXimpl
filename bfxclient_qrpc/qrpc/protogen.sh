#!/bin/bash

SOURCEDIR=.
DESTINATION=./gen

protoc -I=. --cpp_out=$DESTINATION ./messaging.proto