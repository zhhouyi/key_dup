CC = g++ 
CFLAGS = -O3 -Wall -fno-operator-names -std=c++0x
LIBS = -lcrypto -lssl -lpthread -lsnappy 
INCLUDES =-I./leveldb/include/

all: fsl-freq-analysis

fsl-freq-analysis: ./fsl-count.cc
	$(shell ! test -d "target-trace" && mkdir target-trace)
	$(shell ! test -d "fsl-training" && mkdir fsl-training)
	$(shell ! test -d "log" && mkdir log)
	$(shell ! test -d "tmp" && mkdir tmp)
	$(shell ! test -d "db-target" && mkdir db-target)
	$(shell ! test -d "db-training" && mkdir db-training)
	$(shell ! test -d "left-1" && mkdir left-1)
	$(shell ! test -d "right-1" && mkdir right-1)
	$(shell ! test -d "left-2" && mkdir left-2)
	$(shell ! test -d "right-2" && mkdir right-2)
	$(shell ! test -d "uniq-db" && mkdir uniq-db)
	$(CC) $(CFLAGS) -o fsl-count-local fsl-count-local.cc $(INCLUDES) ./leveldb/out-static/libleveldb.a $(LIBS)
	$(CC) $(CFLAGS) -o k-minhash kminhash.cc $(INCLUDES) ./leveldb/out-static/libleveldb.a $(LIBS)
	$(CC) $(CFLAGS) -o mh-count mh-count.cc $(INCLUDES) ./leveldb/out-static/libleveldb.a $(LIBS)
	$(CC) $(CFLAGS) -o mh-stat mh-print.cc $(INCLUDES) ./leveldb/out-static/libleveldb.a $(LIBS)

clean:
	@rm -f fsl-count
	@rm -f db-stat
	@rm -f fsl-count-local
	@rm -f db-stat-local
	@rm -f db-rank
	@rm -f freq-analysis
	@rm -f db-dedup
	@rm -f k-minhash
	@rm -f mh-count
	@rm -f mh-stat
	@rm -f log/*
	@rm -f tmp/*
	@rm -f $(MAIN_OBJS)
	@rm -rf db-target/*
	@rm -rf db-training/*
	@rm -rf left-1/*
	@rm -rf right-1/*
	@rm -rf left-2/*
	@rm -rf right-2/*
	@rm -rf uniq-db/*
	@rm -rf db-relate/*
