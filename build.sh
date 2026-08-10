#!/bin/bash
xhost +local:docker
test ! -d .artifacts && mkdir -p .artifacts/.config && mkdir -p .artifacts/.cache

docker build -t sdk:latest . && \
docker compose build qtdeploy && \
docker compose run --rm qtdeploy        