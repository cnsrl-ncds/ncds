#!/bin/bash
dir=`pwd`
INTERVAL=60
WINDOWS=3
I=2
entry=15
entry_k=32
hash_number=5
##DISCOUT=0.2
## 1w prediction list with 0.5 discount inw1 and w2
###1014 should compile kary.c not t1.c
for threshold in 0.01 
##0.02 0.05 0.1
do

	## Create a new Makefile
	rm -rf Makefile
	cp Makefile_original Makefile

	echo '	$(CC) $(CFLAGS) -o kary $(LIBDIR)/*.c $(SRCDIR)/kary.c  $(LIBS) -D Debug=1 -D INTERVAL='$INTERVAL" -D WINDOWS="$WINDOWS" -D HASH="$hash_number" -D KEY_bit="$entry" -D T="$threshold>> Makefile

	cat Makefile_attach >> Makefile

	## Into simulation folder
	mkdir $entry_k"K_"$hash_number"H_"$INTERVAL"s_"$threshold"t"
	mv Makefile ./$entry_k"K_"$hash_number"H_"$INTERVAL"s_"$threshold"t"

	## Simulation
	cd $entry_k"K_"$hash_number"H_"$INTERVAL"s_"$threshold"t"
	make
	arch > log
	./kary ../small.txt >> log

	cd ..
done
