#!/bin/bash

DAOGRAPH_IMAGE_NAME="daograph_env"
DAOGRAPH_DIR="$(cd "$(dirname "$0")" && pwd)"
MY_CONTAINER_NAME="DaoGraph_${USER}"

if [ "$(uname)" != "Linux" ]; then
    echo -e "\033[33mPlease work in Linux env. Stop...\033[0m"
    exit 0
fi

# 判断 daograph_env 镜像是否存在，没有就创建
if ! docker images "$DAOGRAPH_IMAGE_NAME" | grep -q "$DAOGRAPH_IMAGE_NAME"; then
    docker build -f "$DAOGRAPH_DIR"/dockerfile -t $DAOGRAPH_IMAGE_NAME "$DAOGRAPH_DIR"
    echo -e "\033[32mBuild $DAOGRAPH_IMAGE_NAME success.\033[0m"
fi

# 如果未开启，则开启并且进入
if [ -z "$(docker ps -q -f name="$MY_CONTAINER_NAME")" ]; then
    docker run -it --rm --name="$MY_CONTAINER_NAME" -v "$DAOGRAPH_DIR":/home/DaoGraph "$DAOGRAPH_IMAGE_NAME" /bin/bash
else
    echo -e "\033[33mContainer ${MY_CONTAINER_NAME} is still running.\033[0m"
fi
