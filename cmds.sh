protoc --c_out=. amessage.proto
clang -lprotobuf-c amessage_pack.c amessage.pb-c.c -o pack
clang -lprotobuf-c amessage_unpack.c amessage.pb-c.c -o unpack


protoc --plugin=protoc-gen-nanopb=nanopb/generator/protoc-gen-nanopb --nanopb_out=. amessage.proto
nanopb-0.3.9-linux-x86/generator-bin/protoc --nanopb_out=. amessage.proto
clang -ggdb -Inanopb-0.3.9-linux-x86 -Igen nanopb-0.3.9-linux-x86/{pb_encode.c,pb_decode.c,pb_common.c} scriptSend.c gen/amessage.pb.c -o send
clang -ggdb -Inanopb-0.3.9-linux-x86 -Igen nanopb-0.3.9-linux-x86/{pb_encode.c,pb_decode.c,pb_common.c} recv.c gen/amessage.pb.c -o recv


