FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
ARG USERNAME=builder

RUN apt update && apt install -y --no-install-recommends \
    python3 \
    xz-utils \
    file \
    cmake \
    make \
    ninja-build \
    dpkg-dev \
    file \
    && apt clean \
    && rm -rf /var/apt/lists/*

RUN --mount=type=bind,target=/home/sdk,source=sdk,readwrite \
    cd /home/sdk \
    && chmod +x *.sh \
    && ./*.sh \
    && ln -s /opt/sigmastudio/* /toolchain

RUN groupadd -g 1000 ${USERNAME} \ 
    && useradd -u 1000 -g ${USERNAME} -m ${USERNAME} \
    && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers