#!/bin/sh

rm localoutput.csv 2>/dev/null
rm -r logs 2>/dev/null
mkdir logs 2>/dev/null

echo loop,thread_num,schedule,time,value > output.csv
for thread_num in 1 2 4 6 8 12 16
do
  export OMP_NUM_THREADS=$thread_num
  echo -----Thread Number $thread_num------
  for binfile in $(ls ./bin)
  do
    echo executing $binfile ......
    ./bin/$binfile >> localoutput.csv
  done
done

