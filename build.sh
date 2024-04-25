rm -r src/*
rm -r build/*

mkdir src
mkdir build

asn1c RRC.asn1 -D src -no-gen-OER -no-gen-example

gcc -Isrc -o build/client client.c asn1.c src/*.c -DASN_DISABLE_OER_SUPPORT -lsctp
gcc -Isrc -o build/server server.c asn1.c src/*.c -DASN_DISABLE_OER_SUPPORT -lsctp