#include <assert.h>
#include <stdio.h>
#include <queue>
#include <sstream>
#include <string.h>
#include <string>
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
#define FP_SIZE2 16
#define K_MINHASH 1
#define SEG_SIZE ((2<<20)) //1MB default
#define SEG_MIN ((2<<19)) //512KB
#define SEG_MAX ((2<<21)) //2MB

using namespace std;

struct node{
	char key[FP_SIZE];
	uint64_t size;
};

/*struct seg{
       char key[FP_SIZE2];
       uint64_t size;
};*/

leveldb::DB *db;
queue<node> sq;
//vector<seg> sq2;
string s;
void init_relate(char *rel){
      leveldb::Options options;
      options.create_if_missing = true;
      leveldb::Status status = leveldb::DB::Open(options, rel, &db);
      assert(status.ok());
        assert(db != NULL);
}


uint64_t key_num=0;


void process_seg(int size)   //duplication
{  
    char hash2[FP_SIZE2];
    unsigned char md5full[FP_SIZE2*4];
    //const char *p=s.c_str();
   /*char hash3[FP_SIZE*4000];
    for(int i=0;i<=s.length();i++){
        hash3[i]=s[i];
    }*/
    MD5((unsigned char*)s.c_str(), s.length(),md5full);
    memcpy(hash2,md5full,FP_SIZE2);
    /* for(int i=0;i<FP_SIZE2;i++){
   	printf(":%.2hhx", md5full[i]);
    }
    printf("\t%d\n",size);*/
	string key(hash2, FP_SIZE2);
	string exs="";
    leveldb::Status statu = db->Get(leveldb::ReadOptions(),key,&exs);
    if(statu.ok()==0){
        statu=db->Put(leveldb::WriteOptions(),key,s);
            key_num++;
    }
}


void read_hashes(FILE *fp) {
	char read_buffer[256];
	char *item;
	char last[FP_SIZE];
	memset(last, 0, FP_SIZE);// make all elements in last as 0
        uint64_t sq_size=0;

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
                string s1(hash, FP_SIZE);
		uint64_t size = atoi((const char*)item);   //string-->int
                //uint64_t size=strlen(item);
                         //get the segment
                //s1.assign(hash, hash+FP_SIZE);
                s=s+s1;

		if (sq_size + size > SEG_MAX || (sq_size >= SEG_MIN && (hash[5] << 2) >> 2 == 0x3f)){
			//seg.size=sq_size+size;
                        process_seg(sq_size);
			//while(!pq.empty()) pq.pop();
			while(!sq.empty()) sq.pop();
                        s="";                 //clear string
			sq_size = 0;
		}

		node entry;
		memcpy(entry.key, hash, FP_SIZE);
		entry.size = size;
		sq_size += size;
		sq.push(entry);
		
	}
}

int main (int argc, char *argv[]){
	assert(argc >= 2);  //assert=if ... end procedure
	// argv[1] points to hash file; argv[2] points to analysis db  
	FILE *fp = NULL;
	fp = fopen(argv[1], "r");
	assert(fp != NULL);
	init_relate("./count_key1-db/");
	read_hashes(fp);
	fclose(fp);
	return 0;
}
