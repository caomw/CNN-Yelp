CC = g++
CFLAGS = -O2 -Wall -Wno-unused-result

all:	embedding info info2 genmulti genbinary1245 genbinary15 filter
	
embedding:	embedding.cc
	$(CC) $(CFLAGS) embedding.cc -o embedding

filter:	filter.cc
	$(CC) $(CFLAGS) filter.cc -o filter

info: info.cc
	$(CC) $(CFLAGS) info.cc -o info

info2: info2.cc
	$(CC) $(CFLAGS) info2.cc -o info2
	
genmulti: genmulti.cc
	$(CC) $(CFLAGS) genmulti.cc -o genmulti

genbinary1245: genbinary1245.cc
	$(CC) $(CFLAGS) genbinary1245.cc -o genbinary1245

genbinary15: genbinary15.cc
	$(CC) $(CFLAGS) genbinary15.cc -o genbinary15

clean:
	rm -rf embedding genmulti info info2 genbinary1245 genbinary15 filter
