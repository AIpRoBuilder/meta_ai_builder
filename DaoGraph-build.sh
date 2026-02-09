#!/bin/bash

rm -rf build

cmake -DDAOGRAPH_BUILD_FUNCTIONAL_TESTS=ON -DDAOGRAPH_BUILD_PERFORMANCE_TESTS=ON . -Bbuild
cd build || { ! echo "enter build failed"; exit 1; }
make -j8
echo -e "\033[34m congratulations, automatic compile DaoGraph finish... \033[0m"
