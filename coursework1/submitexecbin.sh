#!/bin/sh

rm submitoutput.csv 2>/dev/null
rm -r sub 2>/dev/null
mkdir sub
cd sub

for thread_num in 1 2 4 6 8 12 16
do
  export OMP_NUM_THREADS=$thread_num
  echo -----Submit thread count: $thread_num ------
  for binfile in $(ls ../bin)
  do
    rm submit_exec.pbs 2>/dev/null
    cp ../submit_template.pbs ./submit_exec.pbs
    sed -i "s/TEMPLATE_NUM/$thread_num/g" ./submit_exec.pbs
    sed -i "s#TEMPLATE_NAME#bin/$binfile#g" ./submit_exec.pbs
    echo submitting $binfile ......
    qsub -q S136821 ./submit_exec.pbs
  done
done

