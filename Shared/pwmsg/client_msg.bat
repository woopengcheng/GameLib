"./ProbuffAssembly_client.exe"  ./proto
"../../server/bin/protoc.exe" --cpp_out=../../server/tools/src/NetCore/MessageLib/msg ./client.proto
"../../server/tools/src/NetCore/Debug/pw_proto_cpp_gen.exe" ./client.proto ../../server/tools/src/NetCore/MessageLib/msg pwclient
