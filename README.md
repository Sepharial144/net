# Net
Net it is a library for network programming.
The library contain sockets for Win32 and Linux.

Windows build system
cmake . -Bbuild

# build debug mode
cmake --build build --config Debug

# build release mode
cmake --build build --config Release

cmake --isntall .\build --prefix "include"