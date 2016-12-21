#!/bin/sh

file_list=`ls -a | sort`

SAVEIFS=$IFS						#restore environment vars
IFS=$(echo -en "\n\b")

for file_a in $file_list;
do
   for file_b in $file_list;
   do
	if [ $file_a = $file_b ]
	then
		continue
	fi
	echo "Processing:"${file_a}" & "${file_b};
	filediff=comm $file_a $file_b -3
	if [ z $filediff ];
		then
			echo "yes, same";
	fi
   done
done

IFS=$SAVEIFS
