#!/bin/bash
DIRECTORY=../result/openmp2gb_take2
mkdir -p $DIRECTORY
rm $DIRECTORY/core*
for i in 24 16 8 4 2 1
do
  export OMP_NUM_THREADS=$i
  for j in {1..10}
  do
    echo "cores:	$i	run:	$j" >> $DIRECTORY/cores.$i.txt
    aprun -n 1 -d $i ./../bin/search_openmp >> $DIRECTORY/cores.$i.txt
    wait
  done
done

DIRECTORY=../result/mpio2gb_take2
mkdir -p $DIRECTORY
rm $DIRECTORY/core*
for i in 24 16 8 4 2 1
do
  for j in {1..10}
  do
    echo "cores:        $i      run:    $j" >> $DIRECTORY/cores.$i.txt
    aprun -n $i ./../bin/search_mpio >> $DIRECTORY/cores.$i.txt
    wait
  done
done

