# Net
Net it is a library for network programming.
The library contain sockets for Win32 and Linux.

Windows build system
cmake . -Bbuild

# build debug mode
cmake --build build --config Debug

# build release mode
cmake --build build --config Release

cmake --install .\build --prefix "include"



# Windows compilation
cmake . -Bbuild -G "Visual Studio 17 2022"

# build debug mode
cmake --build build --config Debug

# build release mode
cmake --build build --config Release

cmake --install build --config Debug --prefix "include"