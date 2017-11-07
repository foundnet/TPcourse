#!/bin/sh
rm -r src 2>/dev/null
rm -r bin 2>/dev/null
mkdir src 2>/dev/null
mkdir bin 2>/dev/null

for schedule in static auto static, dynamic, guided,
do
  echo -----$schedule catagory-----
  if [ ${schedule:0-1:1} = "," ]; then
    for block in 1 2 4 8 16 32 64
    do
      srep=${schedule%?}-$block
      echo creating $srep ......
      cp loops_temp.c ./src/loops_$srep.c
      sed -i "s/SCHEDULE_NAME/$srep/g" ./src/loops_$srep.c
      sed -i "s/SCHEDULE_DIREC/$schedule$block/g" ./src/loops_$srep.c
      echo icc -o ./bin/loops_$srep ./src/loops_$srep.c -qopenmp -std=c99 $1
      icc -o ./bin/loops_$srep ./src/loops_$srep.c -qopenmp -std=c99 $1
    done
  else
    echo creating $schedule ......
    cp loops_temp.c ./src/loops_$schedule.c
    sed -i "s/SCHEDULE_NAME/$schedule/g" ./src/loops_$schedule.c
    sed -i "s/SCHEDULE_DIREC/$schedule/g" ./src/loops_$schedule.c
    echo icc -o ./bin/loops_$schedule ./src/loops_$schedule.c -qopenmp -std=c99 $1
    icc -o ./bin/loops_$schedule ./src/loops_$schedule.c -qopenmp -std=c99 $1
  fi
done

