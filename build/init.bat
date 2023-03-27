@echo off
echo "开始安装vcpkg..."
set root=%cd%
D:
cd ProgramData
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg integrate install
.\vcpkg\vcpkg integrate project
echo "安装vcpkg完成..."