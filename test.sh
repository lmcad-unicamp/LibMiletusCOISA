#!/bin/bash

echo ">> Entering 'linux' directory"
cd linux

if [ ! -d third-party ]; then
    THIRD_PARTY_ALREADY_EXISTS=false
    echo ">> Creating 'third-party' directory"
    mkdir third-party
else
    THIRD_PARTY_ALREADY_EXISTS=true
fi

echo ">> Entering 'third-party' directory"
cd third-party

if [ ! -d abi-compliance-checker ]; then
    git clone https://github.com/lvc/abi-compliance-checker.git

    echo ">> Entering 'abi-compliance-checker' directory"
    cd abi-compliance-checker

    echo ">> Running make"
    sudo make install prefix=/usr

    echo ">> Leaving 'abi-compliance-checker' directory"
    cd ../
else
    echo ">> The 'abi-compliance-checker' directory already exists. Moving on..."
fi

if [ ! -d api-sanity-checker ]; then
    git clone https://github.com/lvc/api-sanity-checker.git

    echo ">> Entering 'api-sanity-checker' directory"
    cd api-sanity-checker

    echo ">> Running make"
    sudo make install prefix=/usr

    echo ">> Leaving 'api-sanity-checker' directory"
    cd ../
else
    echo ">> The 'api-sanity-checker' directory already exists. Moving on..."
fi

echo ">> Leaving 'third-party' directory"
cd ../

echo ">> Entering 'bin' directory"
cd bin

echo ">> Recreating shared library"
rm -f sharedlibrary.so
g++ *.o -shared -o sharedlibrary.so -std=c++11

echo ">> Leaving 'bin' directory"
cd ../

echo ">> Leaving 'linux' directory"
cd ../

echo ">> Generating, building, and running test cases"
api-sanity-checker -l libMiletus -d VERSION.xml -relpath ${PWD} -gen -build -run

echo ">> Done! Expected success/fail: generating = 88/1, building = 87/1, running = 87/0"