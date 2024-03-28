THIRDPARTY_FOLDER=${PWD}
sudo apt-get install libudev-dev -y
cd ${THIRDPARTY_FOLDER}/libusb
sh bootstrap.sh
./configure  --disable-udev 
make
make install

cd ${THIRDPARTY_FOLDER}/libuvc
mkdir -p build/
cd build
cmake ..
make 
make install

mkdir -p ${THIRDPARTY_FOLDER}/libs/
mkdir -p ${THIRDPARTY_FOLDER}/libs/linux/
cp -f ${THIRDPARTY_FOLDER}/libusb/libusb/.libs/libusb-1.0.a ${THIRDPARTY_FOLDER}/libs/linux/
cp -f ${THIRDPARTY_FOLDER}/libuvc/build/libuvc.a ${THIRDPARTY_FOLDER}/libs/linux/
cp -r ${THIRDPARTY_FOLDER}/libuvc/include/libuvc  ${THIRDPARTY_FOLDER}/libs/linux/
cp -r ${THIRDPARTY_FOLDER}/libuvc/build/include/libuvc/libuvc_config.h  ${THIRDPARTY_FOLDER}/libs/linux/libuvc/