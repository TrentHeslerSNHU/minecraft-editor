#include "leveldb/db.h"
#include "nbt.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    //FIXME: Delete this ternary before pushing to prod
    string path = (argc > 1) ? argv[1] : "/home/trent/BedrockWorlds/Townhouse/";
    path += "db";
    ofstream out;
    
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = false;
    leveldb::Status status = leveldb::DB::Open(options, path, &db);
    if(!status.ok()) {
        cout << "Couldn't open level db! (" << path << ")" << endl;
    } else {
        cout << "Found the level db! (" << path << ")" << endl;
    }
    assert(status.ok());

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());

    for(it->SeekToFirst(); it->Valid(); it->Next()){
        if(!status.ok()){
            break;
        }

        leveldb::Slice key = it->key();
        leveldb::Slice value = it->value();

        if(key.ToString().find("player_") != -1 || key.ToString().find("~local_player") != -1){
            TagCompound root = parseNBT(value.ToString());
            root.listChildren();
        }

    }

    assert(status.ok());
    delete it;
    delete db;
    
    return 0;
}