CC = g++ 
CFLAGS = -O3 -Wall -fno-operator-names -std=c++0x
LIBS = -lcrypto -lssl -lpthread -lsnappy 
INCLUDES =-I./leveldb/include/
MAIN_OBJS = count_key.o

all: count_key 

count_key: ./count_key.cc
	$(CC) $(CFLAGS) -o c_key count_key.cc $(INCLUDES) ./leveldb/out-static/libleveldb.a $(LIBS)
	$(CC) $(CFLAGS) -o key_num numcount.cc $(INCLUDES) ./leveldb/out-static/libleveldb.a $(LIBS)
clean:
	@rm -f c_key
	@rm -f key_num
