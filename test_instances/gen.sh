#!/bin/bash
g++ gen.cpp
for i in {0..30}
do
	./a.out $i > ../../input/input$i.txt
	echo "Done input $i"
done
