#include <iostream>
#include <fstream>
#include <cstdio>
#include <filesystem>
#include <regex>

#include "hash.h"
#include "arr.h"

#include "json.hpp"

using json = nlohmann::json;
namespace fs = filesystem;


int ReadJsonFile(const string fileName, string& schemaName, int& tuplesLimit, MyHash<string, MyArr<string>*>& jsonStructure, MyArr<string>& keys) {
    ifstream file(fileName);
    if (!file.is_open()) {
        throw runtime_error("Failed to open schema.json");
    }

    json schema;
    file >> schema;

    schemaName = schema["name"];
    fs::create_directory(schemaName);

    tuplesLimit = schema["tuples_limit"];

    json tableStructure = schema["structure"];
    for (auto& [key, value] : tableStructure.items()) {

        fs::create_directory(schemaName + "/" + key);
        MyArr<string>* tempValue = MCREATE<string>(10);
        MSET<string>(keys, key);

        string colNames = key + "_pk";
        for (auto columns : value) {
            colNames += ",";
            string temp = columns;
            colNames += temp;
            MSET(*tempValue, temp);
        }

        HSET<string, MyArr<string>*>(jsonStructure, key, tempValue);

        string Path = schemaName + "/" + key + "/" + key + "_pk_sequence.txt"; 
        ofstream file1(Path);
        file1 << 1;
        file1.close();

        Path = schemaName + "/" + key + "/" + key + "_lock.txt"; 
        ofstream file2(Path);
        file2 << 0;
        file2.close();

        MyArr<string>* Coll = HGET<string, MyArr<string>*>(jsonStructure, key);
        string strcoll;
        for (int i = 0; i < Coll->len; i++) {
            strcoll.push_back(',');
            strcoll += (Coll->data[i]);
        }
        strcoll.push_back('\n');

        Path = schemaName + "/" + key + "/" + "1.csv"; 
        ofstream file3(Path);
        file3 << strcoll;
        file3.close();

    }

    file.close();
    return 0;
}



