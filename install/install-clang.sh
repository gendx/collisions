#!/bin/sh
clang --version
clang++ --version
echo `which clang`
echo `which clang++`
update-alternatives --install /usr/bin/g++ g++ /usr/bin/clang++ 91

