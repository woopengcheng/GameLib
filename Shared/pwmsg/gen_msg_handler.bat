"./ProbuffAssembly.exe"  ./proto
"protoc.exe" --cpp_out=./msg ./accsrv.proto

"pw_proto_cpp_gen.exe" ./accsrv.proto ./msg pwaccsrv
