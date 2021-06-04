test: Test.o
	echo "Build complete"
CreateFolders:
	mkdir -p bin
Test.o: CreateFolders
	gcc -std=gnu11 -pipe -g -o Test main.c
	mv Test bin/Test
	rm -rf *.dSYM

final: Final.o
	echo "Build complete"
Final.o: CreateFolders
	/usr/bin/gcc -DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o Final main.c -lm
	mv Final bin/Final
	rm -rf *.dSYM