CreateFolders:
	mkdir -p source
	mkdir -p bin
Open.o: CreateFolders
	cp main.c source/Open.c
	/usr/bin/gcc -DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o Open source/Open.c -lm
	mv Open bin/Open
	rm -rf *.dSYM
UpTo18.o: CreateFolders
	cp main.c source/UpTo18.c
	/usr/bin/gcc -DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o UpTo18 source/UpTo18.c -lm
	mv UpTo18 bin/UpTo18
	rm -rf *.dSYM
UpTo30.o: CreateFolders
	cp main.c source/UpTo30.c
	/usr/bin/gcc -DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o UpTo30 source/UpTo30.c -lm
	mv UpTo30 bin/UpTo30
	rm -rf *.dSYM
CumLaude.o: CreateFolders
	cp main.c source/CumLaude.c
	/usr/bin/gcc -DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o CumLaude source/CumLaude.c -lm
	mv CumLaude bin/CumLaude
	rm -rf *.dSYM
final: Open.o UpTo18.o UpTo30.o CumLaude.o
	echo "Build complete"

Test.o: CreateFolders
	gcc -O2 -g -o Test main.c
	mv Test bin/Test
	rm -rf *.dSYM
test: Test.o
	echo "Build complete"
