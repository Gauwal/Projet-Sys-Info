#!/usr/bin/bash

gcc Philosopheaa.c sem.c -o philo -lpthread


echo '1,2,3,4,5,6' > philaa.csv
for l in {1..5}
do
    declare -a myarray
    for n in {1..6}
    do
	m=$((2 * $n))
	m=$((1 + $m))
	output=$(/usr/bin/time -f %e ./philo $m 2>&1)
	myarray+=($output)
	
    done
    echo ${myarray[*]} | tr ' ' ',' >> philaa.csv
    unset myarray
done

rm -rf philo

gcc Philosopheaa_tatas.c sem.c -o philo -lpthread


echo '1,2,3,4,5,6' > philaa_tatas.csv
for l in {1..5}
do
    declare -a myarray
    for n in {1..6}
    do
	m=$((2 * $n))
	m=$((1 + $m))
	output=$(/usr/bin/time -f %e ./philo $m 2>&1)
	myarray+=($output)
	
    done
    echo ${myarray[*]} | tr ' ' ',' >> philaa_tatas.csv
    unset myarray
done

rm -rf philo
