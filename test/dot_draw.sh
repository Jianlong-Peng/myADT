#!/bin/bash

if [ $# -ne 1 ];then
	echo "Usage: $0 *.dot"
	exit
fi

for name in `ls $1`
do
	dot -Tpng ${name} -o ${name%dot}png
done
