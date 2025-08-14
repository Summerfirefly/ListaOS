#!/usr/bin/env bash

GCC_VERSION="15.2.0"
BINUTILS_VERSION="2.45"

GCC_DOWNLOAD_MIRROR="https://mirrors.tuna.tsinghua.edu.cn/gnu/gcc"
BINUTILS_DOWNLOAD_MIRROR="https://mirrors.tuna.tsinghua.edu.cn/gnu/binutils"

TARGET="i686-elf"
PREFIX="$HOME/.local/software/i686-elf"

export PATH="$PREFIX/bin:$PATH"
PWD=$(pwd)
BUILDDIR="$PWD/build-toolchain"

function download_source() {
    if [ ! -e "binutils-${BINUTILS_VERSION}.tar.xz" ]
    then
        wget "$BINUTILS_DOWNLOAD_MIRROR/binutils-${BINUTILS_VERSION}.tar.xz"
        if [ $? -ne 0 ]
        then
            echo "Failed to download binutils-${BINUTILS_VERSION}.tar.xz"
            return 1
        fi
    fi

    if [ ! -e "gcc-${GCC_VERSION}.tar.xz" ]
    then
        wget "$GCC_DOWNLOAD_MIRROR/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz"
        if [ $? -ne 0 ]
        then
            echo "Failed to download gcc-${GCC_VERSION}.tar.xz"
            return 2
        fi
    fi
}

function build_binutils() {
    cd "$BUILDDIR"
    tar --xz -xf binutils-${BINUTILS_VERSION}.tar.xz
    mkdir -p build-binutils
    cd build-binutils
    "../binutils-${BINUTILS_VERSION}/configure" --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
}

function build_gcc() {
    cd "$BUILDDIR"
    tar --xz -xf gcc-${GCC_VERSION}.tar.xz
    mkdir -p build-gcc
    cd build-gcc
    "../gcc-${GCC_VERSION}/configure" --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
    make all-gcc && make all-target-libgcc && make all-target-libstdc++-v3
}

function install_binutils() {
    cd "$BUILDDIR"/build-binutils
    make install
}

function install_gcc() {
    cd "$BUILDDIR"/build-gcc
    make install-gcc
    make install-target-libgcc
    make install-target-libstdc++-v3
}

mkdir -p "$BUILDDIR"
cd "$BUILDDIR"
download_source
if [ $? -ne 0 ]
then
    echo "Failed to download source, exit"
    exit 1
fi

build_binutils
if [ $? -ne 0 ]
then
    echo "Failed to build binutils, exit"
    exit 2
fi

install_binutils
if [ $? -ne 0 ]
then
    echo "Failed to install binutils, exit"
    exit 3
fi

build_gcc
if [ $? -ne 0 ]
then
    echo "Failed to build GCC, exit"
    exit 4
fi

install_gcc
if [ $? -ne 0 ]
then
    echo "Failed to install GCC, exit"
    exit 5
fi

cd "$PWD"
rm -rf build-toolchain
