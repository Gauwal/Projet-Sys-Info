#!/usr/bin/bash

gcc Prodconsaa.c sem.c -o Pro -lpthread


echo '1,2,3,4,5,6' > Prodaa.csv
for l in {1..5}
do
    declare -a myarray
    for n in {1..6}
    do
	output=$(/usr/bin/time -f %e ./Pro $n $n 2>&1)
	myarray+=($output)
    done
    echo ${myarray[*]} | tr ' ' ',' >> Prodaa.csv
    unset myarray
    echo $l
done

rm -rf Pro
