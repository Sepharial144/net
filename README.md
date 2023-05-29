# Net version 0.0.1
Net is crossplatform library for network programming.
The library contain sockets for Win32 and Linux.

# Table of Contents
1. [Features](#features)
2. [Installation dependencies](#installation-dependencies)
3. [Build](#third-example)
4. [Linux examples](#linux-examples)
5. [Windows examples](#windows-examples)

#### Features
| Feature | Linux | Windows |
| ------ | ------ | ------ |
| Asynchronous sockets  | yes | no|
| Synchronous sockets  | yes | yes |
| Header only | no | no |
| Static library | yes | no |
| Shared library | yes | no |

#### Installation dependencies
Net library use google test, if you want check api or unit testing follow steps
##### Linux
```sh
apt-get install libgtest-dev
```
##### Windows
Install vcpkg package manager to use google test as global dependency by https://vcpkg.io/en/getting-started.html

```sh
vcpkg install gtest
```
Note: if you have some problems to execute vcpkg on command line add it to PATH variable

## Build variables
Use on of the cmake option to enable feature
| Feature | OPTION |
| ------ | ------ |
| Testing  | -BNET_TESTING=ON |
| Examples | -BNET_EXAMPLES=ON |


### Build library
Linux
```sh
cd net
cmake . -Bbuild
cmake --build build --config Release  # or Debug
```
Windows
```sh
cd net
cmake . -Bbuild
cmake --build build --config Release  # or Debug
```
### Examples
##### Linux
```sh
some code examples
```
##### Windows
```sh
some code examples
```
