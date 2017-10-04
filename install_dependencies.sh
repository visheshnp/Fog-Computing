#get dependencies
sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get -y install autoconf automake libtool curl make g++ unzip build-essential cmake pkg-config build-essential
sudo apt-get -y update
sudo apt-get -y upgrade

#libzmq install
cd ~
git clone https://github.com/zeromq/libzmq
cd libzmq
./autogen.sh
./configure
make check
sudo make install
sudo ldconfig

#zmq.hpp copy to folder
git clone https://github.com/zeromq/cppzmq.git
cd /usr/local/include
cp ~/cppzmq/zmq.hpp .

#protobuf
wget https://github.com/google/protobuf/releases/download/v3.2.0/protobuf-cpp-3.2.0.tar.gz
tar -xvzf protobuf-cpp-3.2.0.tar.gz
cd protobuf-3.2.0/
./autogen.sh
./configure
make
make check
sudo make install
sudo ldconfig