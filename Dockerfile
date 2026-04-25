FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y --no-install-recommends \
    python3 \
    xz-utils \
    file \
    cmake \
    make \
    ninja-build \
    && apt clean \
    && rm -rf /var/apt/lists/*

RUN --mount=type=bind,target=/home/sdk,source=sdk,readwrite \
    cd /home/sdk \
    && chmod +x *.sh \
    && ./*.sh