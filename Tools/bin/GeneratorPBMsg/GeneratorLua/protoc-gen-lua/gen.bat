@SET OUT_DIR=../../../../../bin/vs14.0/x64/Lib_Debug_x64/lua/msg
@SET OPTION= --lua_out=%OUT_DIR% --plugin=protoc-gen-lua="protoc-gen-lua.bat" --proto_path=../protos

@rem for test
@rem protoc ./Protoc/*.proto --lua_out=%OUT_DIR% --plugin=protoc-gen-lua="protoc-gen-lua.bat"

protoc ../protos/google/protobuf/*.proto %OPTION%
protoc ../protos/PB/*.proto %OPTION%

@pause
