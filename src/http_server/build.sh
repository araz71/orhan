echo "********* Build http-server *********"
cd cpp
rm -rf bin
mkdir bin
cd bin
cmake ../
cd ../../

cd rust
cargo build
cd ../