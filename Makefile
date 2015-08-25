CC = g++
CFLAGS = -O2 -Wall -Wno-unused-result

all:	embedding info info2 gensample genbinary
	
embedding:	embedding.cc
	$(CC) $(CFLAGS) embedding.cc -o embedding

info: info.cc
	$(CC) $(CFLAGS) info.cc -o info

info2: info2.cc
	$(CC) $(CFLAGS) info2.cc -o info2
	
gensample: gensample.cc
	$(CC) $(CFLAGS) gensample.cc -o gensample

genbinary: genbinary.cc
	$(CC) $(CFLAGS) genbinary.cc -o genbinary

clean:
	rm -rf embedding gensample info info2
