#!/bin/bash
cmake . -Bbuild -DNET_EXAMPLES=ON  -DNET_TESTS=ON && cmake --build build -j4