#!/usr/bin/bash

gcc Philosopheaa.c -o philo -lpthread


echo '1,2,3,4,5,6' > philaa.csv
for l in {1..5}
do
    declare -a myarray
    for n in {1..6}
    do
	m=$((2 * $n))
	output=$(/usr/bin/time -f %e ./philo $m 2>&1)
	myarray+=($output)
    done
    echo ${myarray[*]} | tr ' ' ',' >> philaa.csv
    unset myarray
done

rm -rf philo
