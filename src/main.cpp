/*
Example NBT parser use case displaying all player info for a given world.
*/

#include "leveldb/db.h"
#include "nbt.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    // Either open level path passed in, or default to opening New World
    string path = (argc > 1) ? argv[1] : "./New World/";
    path += "db";
    
    // Open the leveldb, do not create one if non-existent
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = false;
    
    // Check to make sure leveldb opened correctly
    leveldb::Status status = leveldb::DB::Open(options, path, &db);
    if(!status.ok()) {
        cout << "Couldn't open level db! (" << path << ")" << endl;
        return EXIT_FAILURE;
    } else {
        cout << "Found the level db! (" << path << ")" << endl;
    }
    assert(status.ok());

    // Iterate through keys in leveldb
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for(it->SeekToFirst(); it->Valid(); it->Next()){
        if(!status.ok()){
            break;
        }

        // For each key, get the key name
        leveldb::Slice key = it->key();

        // Check key name for prefix indicating player data
        if(key.ToString().find("player_") != -1 || key.ToString().find("~local_player") != -1){
            // Get key value
            leveldb::Slice value = it->value();

            // Optional: output player data to .nbt files
            ofstream data;
            data.open(key.ToString() + ".nbt");
            data << value.ToString();
            
            // Parse NBT player data
            TagCompound* test = parseNBT(value.ToString());
            std::cout << test->numChildren() << " tags\n'" << toNBT(test) << "'" << std::endl;
        }

    }

    // Make sure we left the database in a clean state, then tidy up
    assert(status.ok());
    delete it;
    delete db;
    
    return EXIT_SUCCESS;
}