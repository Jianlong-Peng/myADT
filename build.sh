#!/bin/bash

if [ $# -ne 2 ];then
	echo "Usage: $0 [Release|Debug] [Static|Shared]"
	exit
fi

if [[ $1 != "Release" && $1 != "Debug" ]];then
	echo "Only Release or Debug allowed, but '$1' is given"
	exit
fi

if [[ $2 == "Static" ]];then
	${build_shared}="OFF"
elif [[ $2 == "Shared" ]];then
	${build_shared}="ON"
else
	echo "Only Static or Shared allowed, but '$2' is given"
	exit
fi

if [ ! -d $1 ]; then
	mkdir $1
fi

cd $1

if [ -f CMakeCache.txt ]; then
    rm CMakeCache.txt
fi

cmake .. -DCMAKE_INSTALL_PREFIX=~/jlpeng/bin/template.void -DBUILD_SHARED=${build_shared} -DCMAKE_BUILD_TYPE=$1
cd ..

