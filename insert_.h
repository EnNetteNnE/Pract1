#include <iostream>
#include <fstream>

#include "hash.h"
#include "arr.h"

#include "lock.h"
#include "help.h"

using namespace std;


int Insert(string str, MyHash<string, MyArr<string>*>& jsonStructure, MyHash<string, int>& numb, string schemaName, int tuplesLimit, MyArr<string>& keys) {
    
    string key;
    int i = 12;
    while (str[i] != ' ') {
        key.push_back(str[i]);
        i++;
    }
    
    if (i >= str.size() or SizeIndex(str, i + 1, i + 7) != "VALUES") {
        cout << "Попробуйте ввести запрос ещё раз)" << '\n';
        return 0;
    }
    
    int tableHere = 0;
    for (int j = 0; j < keys.len; j++) {
        if (keys.data[j] == key) {
            tableHere = 1;
            break;
        }
    }
    
    if (tableHere == 0) {
        cout << "Такой таблицы нет" << '\n';
        return 0;
    }

    Lock1(schemaName, key);
   
    string instr;
    int kol = 1;
    i += 7;
    while (i != str.size()) {
        if ((str[i] != ' ') and (str[i] != '\'') and (str[i] != '(') and (str[i] != ')')) {
            instr.push_back(str[i]);
        }       
        if (str[i] == ',') {
            kol++;
        }
        i++;
    }

    MyArr<string>* S = HGET<string, MyArr<string>*>(jsonStructure, key);
    int kolrite = S->len;
    if (kol != kolrite) {
        cout << "Неверное колличество элементов" << '\n';
        return 0;
    }

    string filename = schemaName + "/" + key + "/" + key + "_pk_sequence.txt"; 
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
    }

    string line;
    getline(file, line);
    file.close();

    int lineint = atoi(line.c_str());

    ofstream file2(filename);
    file2 << lineint + 1;
    file2.close();

    int number = HGET<string, int>(numb, key);
    if (lineint > tuplesLimit) {
        HSET<string, int>(numb, key, number + 1);
        number++;
    }

    instr = line + "," + instr + "\n";
    string strn = to_string(number);
    filename = schemaName + "/" + key + "/" + strn + ".csv";
    AddInFile(filename, instr);

    Lock0(schemaName, key);

    return 0;
}
