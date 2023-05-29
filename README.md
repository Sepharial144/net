# Net version 0.0.1
Net is a crossplatform library for network programming.
The library contain socket api for Win32 and Linux.

## Table of Contents
1. [Features](#features)
2. [Installation dependencies](#installation-dependencies)
3. [Build](#build-library)
4. [Examples](#examples)
5. [Windows examples](#windows-examples)

## Features
| Feature | Linux | Windows |
| ------ | ------ | ------ |
| Asynchronous sockets  | yes | no|
| Synchronous sockets  | yes | yes |
| Header only | no | no |
| Static library | yes | no |
| Shared library | yes | no |

## Installation dependencies
Net library use google test, if you want check coverage api or unit tests follow steps

#### Linux
```sh
apt-get install libgtest-dev
```

#### Windows
Make sure you have installed Visual Studio 17 or higher
Install vcpkg package manager to install google test globaly by https://vcpkg.io/en/getting-started.html
```sh
vcpkg install gtest
```
Note: if you have some problems to execute vcpkg on command line add it to PATH variable

## Build library
```sh
git clone https://github.com/Sepharial144/net.git
cd net
cmake . -Bbuild [net_lib_build_options]
cmake --build build --config Release -j # or --config Debug
```

### Build options
Use on of the cmake option to enable feature
| Feature | Option | Default Value |
| ------ | ------ | ------ |
| Testing  | -DNET_TESTING=ON | Off |
| Examples | -DNET_EXAMPLES=ON | Off |

## Examples
Actual examples contain in examples folder


## Net API
Some information about using library
