#!/bin/bash
#
while [ 1 ]
do
	R=$(($RANDOM%9))
	G=$(($RANDOM%9))
	B=$(($RANDOM%9))

	#echo $R$G$B

	./sender $1 $R $G $B
	sleep .025
done 
