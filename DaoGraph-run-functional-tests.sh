#!/bin/bash

DAOGRAPH_TEST_FUNCTIONAL_DIR="build/test/Functional/"

if [ ! -d $DAOGRAPH_TEST_FUNCTIONAL_DIR ]; then
    echo -e "\033[31m[ERROR] Directory $DAOGRAPH_TEST_FUNCTIONAL_DIR does not exist.\033[0m"
    exit 1
fi

# shellcheck disable=SC2164
cd $DAOGRAPH_TEST_FUNCTIONAL_DIR

for file in *; do
    # 执行所有可执行文件，并且过滤掉文件夹
    if [ -f "$file" ] && [ -x "$file" ]; then
        echo "**** Running $file ****"
        "./$file"
        echo " "
    fi
done

echo -e "\033[34m congratulations, automatic run DaoGraph functional test finish... \033[0m"
