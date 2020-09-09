from ubuntu:18.04

workdir /root
run apt-get update && apt-get install -y wget gnupg
run wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

run echo '\n\
deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic main\n\
deb-src http://apt.llvm.org/bionic/ llvm-toolchain-bionic main\n\
deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-8 main\n\
deb-src http://apt.llvm.org/bionic/ llvm-toolchain-bionic-8 main\n' >>/etc/apt/sources.list

run apt-get update && apt-get install -y \
    autoconf2.13        \
    build-essential     \
    bzip2               \
    clang-8             \
    lld-8               \
    lldb-8              \
    pkg-config          \
    zlib1g-dev          \
    libmysqlclient-dev  \
    libssl-dev          \
    curl                \
    libcurl4-openssl-dev \
    libcrypto++-dev \
    zlib1g-dev

run update-alternatives --install /usr/bin/clang clang /usr/bin/clang-8 100
run update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-8 100

workdir /root
run wget https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.tar.gz
run tar xf boost_1_70_0.tar.gz
workdir /root/boost_1_70_0
run ./bootstrap.sh
run ./b2 toolset=clang -j10 install

workdir /root
run wget https://github.com/Kitware/CMake/releases/download/v3.14.5/cmake-3.14.5.tar.gz
run tar xf cmake-3.14.5.tar.gz
workdir /root/cmake-3.14.5
run ./bootstrap --parallel=10
run make -j10
run make -j10 install

workdir /root
run wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.2.4.tar.gz
run tar -zxvf mysql++-3.2.4.tar.gz
run cd mysql++-3.2.4
run ./configure --prefix=/usr/opt/mysql++ --with-mysql-lib=/usr/lib/x86_64-linux-gnu
run make
run make install
run find . -name "*.o" | xargs -i ar rcu libmysqlpp.a {}
run cp libmysql*.a /usr/opt/mysql++/lib/


workdir /root
run mkdir /root/history-tools
copy cmake /root/history-tools/cmake
copy CMakeLists.txt /root/history-tools
copy external /root/history-tools/external
copy libraries /root/history-tools/libraries
copy src /root/history-tools/src
copy wasms /root/history-tools/wasms

