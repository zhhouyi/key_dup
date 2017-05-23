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
leveldb::DB *db;
int main()
{
    uint64_t num=0;
    char*rel="./count_key1-db/";
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, rel, &db);
    assert(status.ok());
    assert(db != NULL);
    //open leveldb

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()){
    	num++;
    }
    assert(it->status().ok());
    delete it;
    //count

    printf("the num of key is %lu",num);
    return 0;
}
