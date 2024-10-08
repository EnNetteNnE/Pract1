#include "insert_.h"


int Delet(string strDF, MyHash<string, MyArr<string>*>& jsonStructure, MyHash<string, int>& numb, string schemaName, MyArr<string>& keys) {

    string key;
    int i = 12;
    while (i != strDF.size()) {
        key.push_back(strDF[i]);
        i++;
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

    string strW;
    getline(cin, strW);

    if (SizeIndex(strW, 0, 6) != "WHERE ") {
        cout << "Попробуйте ввести запрос ещё раз)" << '\n';
        return 0;
    }

    i = 6;
    string table0;
    while (strW[i] != '.') {
        table0.push_back(strW[i]);
        i++;
    }
    if (table0 != key) {
        cout << "Попробуйте ввести запрос ещё раз)" << '\n';
        return 0;
    }

    string kol;
    i++;
    while (strW[i] != ' ') {
        kol.push_back(strW[i]);
        i++;
    }

    int kolhere = -1;
    MyArr<string>* kolumn = HGET<string, MyArr<string>*>(jsonStructure, key);
    for (int k = 0; k < kolumn->len; k++) {
        if (kolumn->data[k] == kol) {
            kolhere = k;
        }
    }
    if (kolhere == -1) {
        cout << "Такой колонки нет" << '\n';
        return 0;
    }

    string instr;
    while (i < strW.size()) {
        if ((strW[i] != ' ') and (strW[i] != '\'') and (strW[i] != '=')) {
            instr.push_back(strW[i]);
        }
        i++;
    }



    int number = HGET<string, int>(numb, key);
    string strn = to_string(number);
    string filename = schemaName + "/" + key + "/" + strn + ".csv";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 0;
    }

    MyArr<string>* lines = MCREATE<string>(10);
    instr = ',' + instr;
    string line;
    getline(file, line);
    string intemp;
    intemp += line;
    intemp.push_back('\n');
    while (getline(file, line)) {
        MSET<string>(*lines, line);
    }
    file.close();

 
    
    for(i = 0; i < lines->len; i++) {
        line = lines->data[i];
        int zap = 0;
        string strok;
        for(char k : line) {
            if (k == ',') {
                zap++;
            }
            if (zap == (kolhere + 1)) {
                strok.push_back(k);
            }
        }
        if (strok != instr) {
            intemp += line;
            intemp.push_back('\n');
        }
    }



    ofstream file2(filename);
    file2 << intemp;
    file2.close();

    Lock0(schemaName, key);

    return 0;
}
