THIRDPARTY_FOLDER=${PWD}
sudo apt-get install libudev-dev -y
cd ${THIRDPARTY_FOLDER}/libusb
./configure  --disable-udev 
make
make install

cd ${THIRDPARTY_FOLDER}/libuvc
mkdir -p build/
cd build
cmake ..
make 
make install

