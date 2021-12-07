#!/usr/bin/bash

gcc Prodcons.c -o Pro -lpthread


echo '1,2,3,4,5,6' > Prod.csv
for l in {1..5}
do
    declare -a myarray
    for n in {1..6}
    do
	output=$(/usr/bin/time -f %e ./Pro $n $n 2>&1)
	myarray+=($output)
    done
    echo ${myarray[*]} | tr ' ' ',' >> Prod.csv
    unset myarray
    echo $l
done

rm -rf Pro
