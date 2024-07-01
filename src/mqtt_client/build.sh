echo "********* Building mqtt client *********"
cd rust
cargo build
cd ../

cd cpp
rm -rf bin
mkdir bin
cd bin
cmake ../
make
cd ../../