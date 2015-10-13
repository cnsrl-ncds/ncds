#!/bin/bash
dir=`pwd`
INTERVAL=3
WINDOWS=3
I=2
entry=15
entry_k=32
hash_number=3
DISCOUNT=0.5
## 1w prediction list with 0.5 discount inw1 and w2
###1014 should compile kary.c not t1.c
for threshold in 0.01 0.02
###0.05 0.1 0.2 0.4
do

	## Create a new Makefile
	rm -rf Makefile
	cp Makefile_original Makefile

	echo '	$(CC) $(CFLAGS) -o kary10g $(LIBDIR)/*.c $(SRCDIR)/kary_disct.c  $(LIBS) -D Debug=1 -D INTERVAL='$INTERVAL" -D WINDOWS="$WINDOWS" -D HASH="$hash_number" -D KEY_bit="$entry" -D T="$threshold" -D DISCOUNT="$DISCOUNT>> Makefile

	cat Makefile_attach >> Makefile

	## Into simulation folder
	mkdir $entry_k"K_"$hash_number"H_"$INTERVAL"s_"$threshold"t"$DISCOUNT"dist_10g"
	mv Makefile ./$entry_k"K_"$hash_number"H_"$INTERVAL"s_"$threshold"t"$DISCOUNT"dist_10g"

	## Simulation
	cd $entry_k"K_"$hash_number"H_"$INTERVAL"s_"$threshold"t"$DISCOUNT"dist_10g"
	make
	./kary10g ../large10g.txt > log10g &

	cd ..
done
