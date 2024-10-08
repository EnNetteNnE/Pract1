#include "where_.h"


int Select(string strS, MyHash<string, MyArr<string>*>& jsonStructure, MyHash<string, int>& numb, string schemaName, MyArr<string>& keys) {
    strS = SizeIndex(strS, 7, strS.size());
    MyArr<MyArr<string>*>* tableSelect = MCREATE<MyArr<string>*>(10);
    int i = 0;
    string kv;
    while(true) {
        if (i == strS.size() or strS[i] == ',') {
            string key;
            string value;
            int dot = 0;
            for(char j : kv) {
                if (j == '.') {
                    dot = 1;
                }
                else if (dot == 1) {
                    value.push_back(j);
                }
                if (dot == 0) {
                    key.push_back(j);
                }
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

            int kolhere = -1;
            MyArr<string>* kolumn = HGET<string, MyArr<string>*>(jsonStructure, key);
            for (int k = 0; k < kolumn->len; k++) {
                if (kolumn->data[k] == value) {
                    kolhere = k;
                }
            }
            if (kolhere == -1) {
                cout << "Такой колонки нет" << '\n';
                return 0;
            }

            MyArr<string>* kval = MCREATE<string>(10);
            MSET(*kval, key);
            MSET(*kval, value);
            MSET(*tableSelect, kval);

            kv = "";
            i += 2;
        }
        if (i >= strS.size()) break;
        kv.push_back(strS[i]);
        i++;
    }

    string strF;
    getline(cin, strF);

    if (SizeIndex(strF, 0, 5) != "FROM ") {
        cout << "Попробуйте ввести запрос ещё раз)" << '\n';
        return 0;
    }
    strF = SizeIndex(strF, 5, strF.size());

    i = 0;
    while(true) {
        if (i == strF.size() or strF[i] == ',') {

            int tableHere = 0;
            for (int j = 0; j < keys.len; j++) {
                if (keys.data[j] == kv) {
                    tableHere = 1;
                    break;
                }
            }    
            if (tableHere == 0) {
                cout << "Такой таблицы нет" << '\n';
                return 0;
            }

            kv = "";
            i += 2;
        }
        if (i >= strF.size()) break;
        kv.push_back(strF[i]);
        i++;
    }

    string key = tableSelect->data[0]->data[0];
    int number = HGET<string, int>(numb, key);
    string strn = to_string(number);
    string filename = schemaName + "/" + key + "/" + strn + ".csv";
    MyArr<MyArr<string>> redvS = ReadCSV(filename);

    MyArr<string>* column = HGET(jsonStructure, key);
    for (i = 0; i < column->len; i++) {
        if (tableSelect->data[0]->data[1] == column->data[i]) {
            break;
        }
    }
    int numbcolumn = i + 1;

    MyArr<string>* tableS = MCREATE<string>(10);
    for (i = 0; i < redvS.len; i++) {
        string set = redvS.data[i].data[0] + " " + redvS.data[i].data[numbcolumn];
        MSET(*tableS, set);
    }

    key = tableSelect->data[1]->data[0];
    number = HGET<string, int>(numb, key);
    strn = to_string(number);
    filename = schemaName + "/" + key + "/" + strn + ".csv";
    MyArr<MyArr<string>> redvE = ReadCSV(filename);

    column = HGET(jsonStructure, key);
    for (i = 0; i < column->len; i++) {
        if (tableSelect->data[1]->data[1] == column->data[i]) {
            break;
        }
    }
    numbcolumn = i + 1;

    MyArr<string>* tableE = MCREATE<string>(10);
    for (i = 0; i < redvE.len; i++) {
        string set = redvE.data[i].data[0] + " " + redvE.data[i].data[numbcolumn];
        MSET(*tableE, set);
    }

    MyArr<MyArr<string>>* BigCross = MCREATE<MyArr<string>>(10);
    for (i = 0; i < redvS.len; i++) {
        for (int j = 0; j < redvE.len; j++) {
            MyArr<string>* join = MCREATE<string>(10);
            for (int k = 0; k < redvS.data[i].len; k++) {
                MSET(*join, redvS.data[i].data[k]);
            }
            for (int k = 0; k < redvE.data[j].len; k++) {
                MSET(*join, redvE.data[j].data[k]);
            }
            MSET(*BigCross, *join);
        }
    }

    MyArr<MyArr<string>>* Cross = MCREATE<MyArr<string>>(10);
    for (i = 0; i < tableS->len; i++) {
        for (int j = 0; j < tableE->len; j++) {
            MyArr<string>* join = MCREATE<string>(10);
            MSET(*join, tableS->data[i]);
            MSET(*join, tableE->data[j]);
            MSET(*Cross, *join);
        }      
    }



    while (true)
    {
        string strW;
        getline(cin, strW);

        if (strW.size() == 0) {
            cout << "Cross join" << '\n';
            for (i = 0; i < Cross->len; i++) {
                for(int j = 0; j < Cross->data[i].len; j++) {
                    //int out = (12 - Cross->data[i].data[j].size());
                    cout << setw(12) << left << Cross->data[i].data[j];
                }
            cout << '\n';
            }
        }
        else if (SizeIndex(strW, 0, 6) == "WHERE ") {
            Where(*tableSelect, SizeIndex(strW, 6, strW.size()), keys, jsonStructure, *BigCross, *Cross);

            break;
        }
        else {
            cout << "Попробуйте ввести запрос ещё раз)" << '\n';
        }
    }
    


    return 0;
}




