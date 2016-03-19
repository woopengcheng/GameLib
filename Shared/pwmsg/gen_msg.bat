"./ProbuffAssembly.exe"  ./proto
"../../server/bin/protoc.exe" --cpp_out=../../server/pwaccsrv/msg ./accsrv.proto

"../../server/bin/protoc.exe" --cpp_out=../../server/pwworldsrv/msg ./worldsrv.proto
