#!/bin/bash
vcpkgPath="/usr/vcpkg"
echo "开始安装vcpkg..."
mkdir $vcpkgPath
cd $vcpkgPath
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh
echo "安装vcpkg完成..."

echo "开始编译项目..."
if [ -n "$0" ]
then
cmake -B $0 -S . "-DCMAKE_TOOLCHAIN_FILE=$vcpkgPath/scripts/buildsystems/vcpkg.cmake"
cmake --build $0
echo "输入编译项目[$0]编译结束..."
else
echo "无输入编译项目,跳过编译..."
fi