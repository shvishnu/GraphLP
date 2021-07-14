#!/bin/sh
if [ $# -lt 0 ]
  then
    echo "Command Format: bash lpsolver.sh file1.lp file2.lp"
    echo "where file1.lp is the file with quadruple lp constraints and file2.lp is the file with triplet lp constraints"
  else
    python3 solver/mipex2.py $1
    python3 solver/mipex2.py $2
    export f1=$1
    export f2=$2
    export f1="${f1%.lp}.xml"
    export f2="${f2%.lp}.xml"
    python3 solver/convert.py $f1 > temp/quadtemp.txt
    python3 solver/convert.py $f2 > temp/tritemp.txt
    touch solution.txt
    ./build/rev -i input.txt -o solution.txt -q temp/quadtemp.txt -t temp/tritemp.txt
fi
