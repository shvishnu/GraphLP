for i in {1..9}
do
	touch lp-$i.txt
	./a.out -i $i.txt -o lp-$i.txt
done
