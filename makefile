Open:
	cp main.c src/Open.c
	clang -std=gnu11 -g -o Open src/Open.c
	mv Open bin/Open
	rm -rf *.dSYM
UpTo18:
	cp main.c src/UpTo18.c
	clang -std=gnu11 -g -o UpTo18 src/UpTo18.c
	mv UpTo18 bin/UpTo18
	rm -rf *.dSYM
UpTo30:
	cp main.c src/UpTo30.c
	clang -std=gnu11 -g -o UpTo30 src/UpTo30.c
	mv UpTo30 bin/UpTo30
	rm -rf *.dSYM
CumLaude:
	cp main.c src/CumLaude.c
	clang -std=gnu11 -g -o CumLaude src/CumLaude.c
	mv CumLaude bin/CumLaude
	rm -rf *.dSYM
build: Open UpTo18 UpTo30 CumLaude
	echo "Build complete"