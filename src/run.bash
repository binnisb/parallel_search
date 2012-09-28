for i in 24 16 8 4 2 1
do
  export OMP_NUM_THREADS=$i
  for j in {1..10}
  do
    echo "cores:	$i	run:	$j" >> ../result/cores.$i.txt
    aprun -n 1 -d $i ./../bin/search_openmp >> ../result/cores.$i.txt
    wait
  done
done
