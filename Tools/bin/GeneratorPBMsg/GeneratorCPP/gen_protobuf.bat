@SET OUT_DIR=../../../src/RobotCommon/msg
@SET OPTION= --cpp_out=%OUT_DIR%  --proto_path=../protos

"protoc.exe" %OPTION% ../protos/google/protobuf/descriptor.proto
"protoc.exe" %OPTION% ../protos/PB/options.proto
"protoc.exe" %OPTION% ../protos/PB/net_common.proto

