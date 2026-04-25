#!/bin/bash

docker build -t sdk:latest . && \
docker run -it --rm \
        --user 1000:1000 \
        -v .:/home/build \
        -w /home/build \
        sdk:latest \
        bash -c " mkdir -p build install                   
                  cmake -S src -B build -DCMAKE_TOOLCHAIN_FILE=/home/build/toolchain.cmake \
                                        -DCMAKE_BUILD_TYPE=Release \
                                        -DCMAKE_INSTALL_PREFIX=install \
                                        -GNinja \
                  && cmake --build build --target install "                