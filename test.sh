#!/bin/bash

for ((i=0;i<1000;i++))
do
	./a.out
	status=$(echo $?)
	if [[ $status != 0 ]];then
		exit -1
	fi
	sleep 1
done
