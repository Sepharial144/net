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
Net library use google test, if you want check api or unit testing follow steps
#### Linux
```sh
apt-get install libgtest-dev
```
#### Windows
Install vcpkg package manager to install google test globaly by https://vcpkg.io/en/getting-started.html

```sh
vcpkg install gtest
```
Note: if you have some problems to execute vcpkg on command line add it to PATH variable

## Build library
```sh
git clone https://github.com/Sepharial144/net.git
cd net
cmake . -Bbuild
cmake --build build --config Release  # or Debug
```
### Build options
Use on of the cmake option to enable feature
| Feature | Option |
| ------ | ------ |
| Testing  | -BNET_TESTING=ON |
| Examples | -BNET_EXAMPLES=ON |

## Examples
Actual examples contain in examples folder


## Net API
Some information about using library
