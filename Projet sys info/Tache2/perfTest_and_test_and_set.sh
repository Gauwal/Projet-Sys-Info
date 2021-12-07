#!/usr/bin/bash

gcc -g Test_and_Test_and_set.c -lpthread -o Test_and_Test_and_set


echo '1,2,3,4,5,6,7' > Test_and_Test_and_set.csv
for l in {1..5}
do
    declare -a myarray
    for n in {1..7}
    do
	m=$((2 * $n))
	output=$(/usr/bin/time -f %e ./Test_and_Test_and_set $m 2>&1)
	myarray+=($output)
    done
    echo ${myarray[*]} | tr ' ' ',' >> Test_and_Test_and_set.csv
    unset myarray
done

rm -rf Test_and_Test_and_set
