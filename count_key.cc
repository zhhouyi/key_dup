#include <assert.h>
#include <stdio.h>
#include <queue>
#include <string.h>
#include <stdlib.h>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <time.h>
#include <inttypes.h>
#include <vector>
#include "leveldb/db.h"

// #define ANALYSIS_DB "./db/"
#define FP_SIZE 6
#define FP_SIZE2 6
#define K_MINHASH 1
#define SEG_SIZE ((2<<20)) //1MB default
#define SEG_MIN ((2<<19)) //512KB
#define SEG_MAX ((2<<21)) //2MB

using namespace std;

struct node{
	char key[FP_SIZE];
	uint64_t size;
};

struct seg{
       char key[FP_SIZE2];
       uint64_t size;
};

leveldb::DB *relate;
queue<node> sq;
vector<seg> sq2;
char segm[FP_SIZE*256];
void init_relate(char *rel){
      leveldb::Options options;
      options.create_if_missing = true;
      leveldb::Status status = leveldb::DB::Open(options, rel, &relate);
      assert(status.ok());
        assert(relate != NULL);
}
uint64_t sq_size = 0;

/*void process_seg()
{
        char ft[FP_SIZE*2];
        char ret[FP_SIZE];
        while(!sq.empty()){
                node now = sq.front();
                memcpy(ft, now.key, FP_SIZE);
                MD5((unsigned char*)ft, FP_SIZE*2, md5full);
                memcpy(ret, md5full, FP_SIZE);

                leveldb::Status cst;
                leveldb::Slice key(ret, FP_SIZE);
                leveldb::Slice pkey(core.key, FP_SIZE);
                cst = relate->Put(leveldb::WriteOptions(), key, pkey);
                int j;
                printf("%.2hhx", ret[0]);
                for(j = 1; j < FP_SIZE; j++)
                        printf(":%.2hhx", ret[j]);

                printf("\t\t%" PRIu64 " ", now.size);
                printf("\t\t\t10\n");

                sq.pop();
        }
}*/
void process_seg()   //duplication
{
    MD5((unsigned char*)segm, FP_SIZE2,seg.hash)
    vector<seg> ::iterator it;
    for(it=sq2.begin();it!=sq2.end;it++){
        if(*seg.hash!=hash){
            sq2.push_back(seg);
        }
    }
}


void read_hashes(FILE *fp) {
	char read_buffer[256];
	char *item;
	char last[FP_SIZE];
	memset(last, 0, FP_SIZE);// make all elements in last as 0

	while (fgets(read_buffer, 256, fp)) {
		// skip title line
		if (strpbrk(read_buffer, "Chunk")) {
			continue;
		}

		// a new chunk
		char hash[FP_SIZE];
		memset(hash, 0, FP_SIZE);


		// store chunk hash and size
		item = strtok(read_buffer, ":\t\n ");
		int idx = 0;
		while (item != NULL && idx < FP_SIZE){
			hash[idx++] = strtol(item, NULL, 16);
			item = strtok(NULL, ":\t\n");
		}

		uint64_t size = atoi((const char*)item);   //string-->int
                strcat(segm,(const char*)hash);         //get the segment


		if (sq_size + size > SEG_MAX || (sq_size >= SEG_MIN && (hash[5] << 2) >> 2 == 0x3f)){
			seg.size=sq_size+size;
                        process_seg();
			//while(!pq.empty()) pq.pop();
			while(!sq.empty()) sq.pop();
                        memset(segm,0,FP_SIZE_256);
			sq_size = 0;
		}

		node entry;
		memcpy(entry.key, hash, FP_SIZE);
		entry.size = size;
		sq_size += size;
		sq.push(entry);
		/*if (pq.size() < K_MINHASH){
			pq.push(entry);
		}else{
			node max = pq.top();
			if(memcmp(max.key, entry.key, FP_SIZE) > 0){
				pq.pop();
				pq.push(entry);
			}
		}*/
	}
}

int main (int argc, char *argv[]){
	srand((unsigned)time(NULL));
	assert(argc >= 2);  //assert=if ... end procedure
	// argv[1] points to hash file; argv[2] points to analysis db  
	FILE *fp = NULL;
	fp = fopen(argv[1], "r");
	assert(fp != NULL);
	init_relate("./relate-db/");
	read_hashes(fp);
	fclose(fp);
	return 0;
}
