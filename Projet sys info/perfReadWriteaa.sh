#!/usr/bin/bash

gcc ReadWriteaa.c -o Read -lpthread


echo '1,2,3,4,5,6' > ReadWriteaa.csv
for l in {1..5}
do
    declare -a myarray
    for n in {1..6}
    do
	output=$(/usr/bin/time -f %e ./Read $n $n 2>&1)
	myarray+=($output)
    done
    echo ${myarray[*]} | tr ' ' ',' >> ReadWriteaa.csv
    unset myarray
    echo $l
done

rm -rf Read
