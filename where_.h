#include "delete_.h"


int Where (MyArr<MyArr<string>*>& tableSelect, string strW, MyArr<string>& keys, MyHash<string, MyArr<string>*>& jsonStructure, MyArr<MyArr<string>>& BigCross, MyArr<MyArr<string>>& Cross) {
    MyArr<string>* strSplit = MCREATE<string>(10);
    int i = 0;
    string word;
    while (true) {
        if(strW[i] == ' ' or i >= strW.size()) {
            MSET(*strSplit, word);
            i++;
            word = "";
        }
        if (i >= strW.size()) break;
        word.push_back(strW[i]);
        i++;
    }

/*
    for(i = 0; i < strSplit->len; i++) {
        cout << strSplit->data[i] << '\n';
    }*/

    if ((strSplit->len - 3) % 4 != 0) {
        cout << "Попробуйте ввести запрос ещё раз)" << '\n';
        return 0;
    }

    string andor;
    for (i = 0; i < strSplit->len; i++) {
        if (strSplit->data[i] == "=") {
            MDEL(*strSplit, i);
        }
        else if (strSplit->data[i] == "OR") {
            andor.push_back('o');
            MDEL(*strSplit, i);
        }
        else if (strSplit->data[i] == "AND") {
            andor.push_back('a');
            MDEL(*strSplit, i);
        }
    }
/*
    cout << andor << '\n';
    for(i = 0; i < strSplit->len; i++) {
        cout << strSplit->data[i] << '\n';
    }*/

    string keyTS0 = tableSelect.data[0]->data[0];
    string keyTS1 = tableSelect.data[1]->data[0];
    MyArr<string>* koll0 = HGET(jsonStructure, keyTS0);
    int numK0 = koll0->len;
    MyArr<string>* koll1 = HGET(jsonStructure, keyTS1);
    int numK1 = koll1->len;


    MyArr<MyArr<int>*>* IndexRight = MCREATE<MyArr<int>*>(10);
    for (i = 0; i < strSplit->len; i += 2) {
        if (strSplit->data[i + 1][0] == '\'') {
            string key;
            string value;
            int dot = 0;
            for(char j : strSplit->data[i]) {
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

            string word = SizeIndex(strSplit->data[i + 1], 1, strSplit->data[i + 1].size() - 1);
            // 1233456789101112131415

            int numberKolumn = -1;
            if (keyTS0 == key) {
                for (int k = 0; k < koll0->len; k++) {
                    if (koll0->data[k] == value) {
                        numberKolumn = k + 1;
                    }
                }
            }
            else if (keyTS1 == key) {
                for (int k = 0; k < koll1->len; k++) {
                    if (koll1->data[k] == value) {
                        numberKolumn = k + 1 + koll0->len + 1;
                    }
                }
            }

            if (numberKolumn == -1) {
                cout << "Попробуйте ввести запрос ещё раз)" << '\n';
                return 0;
            }

            MyArr<int>* IR = MCREATE<int>(10);
            for (int j = 0; j < BigCross.len; j++) {
                if (BigCross.data[j].data[numberKolumn] == word) {
                    MSET(*IR, j);
                }
            }
            MSET(*IndexRight, IR);
        }
        else {
            string key0;
            string value0;
            int dot = 0;
            for(char j : strSplit->data[i]) {
                if (j == '.') {
                    dot = 1;
                }
                else if (dot == 1) {
                    value0.push_back(j);
                }
                if (dot == 0) {
                    key0.push_back(j);
                }
            }

            string key1;
            string value1;
            dot = 0;
            for(char j : strSplit->data[i + 1]) {
                if (j == '.') {
                    dot = 1;
                }
                else if (dot == 1) {
                    value1.push_back(j);
                }
                if (dot == 0) {
                    key1.push_back(j);
                }
            }

            // 12345678901234567890
            int numberKolumn0 = -1;
            if (keyTS0 == key0) {
                for (int k = 0; k < koll0->len; k++) {
                    if (koll0->data[k] == value0) {
                        numberKolumn0 = k + 1;
                    }
                }
            }
            else if (keyTS1 == key0) {
                for (int k = 0; k < koll1->len; k++) {
                    if (koll1->data[k] == value0) {
                        numberKolumn0 = k + 1 + koll0->len + 1;
                    }
                }
            }

            if (numberKolumn0 == -1) {
                cout << "Попробуйте ввести запрос ещё раз)" << '\n';
                return 0;
            }



            int numberKolumn1 = -1;
            if (keyTS0 == key1) {
                for (int k = 0; k < koll0->len; k++) {
                    if (koll0->data[k] == value1) {
                        numberKolumn1 = k + 1;
                    }
                }
            }
            else if (keyTS1 == key1) {
                for (int k = 0; k < koll1->len; k++) {
                    if (koll1->data[k] == value1) {
                        numberKolumn1 = k + 1 + koll0->len + 1;
                    }
                }
            }

            if (numberKolumn1 == -1) {
                cout << "Попробуйте ввести запрос ещё раз)" << '\n';
                return 0;
            }


            MyArr<int>* IR = MCREATE<int>(10);
            for (int j = 0; j < BigCross.len; j++) {
                if (BigCross.data[j].data[numberKolumn0] == BigCross.data[j].data[numberKolumn1]) {
                    MSET(*IR, j);
                }
            }
            MSET(*IndexRight, IR);

        }
    }

/*
    for (i = 0; i < IndexRight->len; i++) {
        for (int j = 0; j < IndexRight->data[i]->len; j++) {
            cout << IndexRight->data[i]->data[j] << " ";
        }
        cout << '\n';
    }
*/
    //MyArr<MyArr<int>*>* IndexRightA = MCREATE<MyArr<int>*>(10);
    i = 0;
    while (i < andor.size()) {
        if (andor[i] == 'a') {

            MyArr<int>* IR0 = IndexRight->data[i];
            MyArr<int>* IR1 = IndexRight->data[i + 1];
            MyArr<int>* IRout = MCREATE<int>(10);
            for (int j = 0; j < IR0->len; j++) {
                for (int k = 0; k < IR1->len; k++) {
                    if (IR0->data[j] == IR1->data[k]) {
                        MSET(*IRout, IR0->data[j]);
                    }
                }
            }
            
            MREPLACE(*IndexRight, i, IRout);
            MDEL(*IndexRight, i + 1);

            andor = SizeIndex(andor, 0, i) + SizeIndex(andor, i + 1, andor.size());         
        }

        if (i >= andor.size()) break;

        if (andor[i] == 'o') i++;
    }

    MyArr<int>* IRout = MCREATE<int>(10);
    for (i = 0; i < IndexRight->len; i++) {
        for (int j = 0; j < IndexRight->data[i]->len; j++) {
            MSET(*IRout, IndexRight->data[i]->data[j]);
            //cout << IndexRight->data[i]->data[j] << ' ';
        }
    }
    //cout << '\n';
    MyArr<int>* IRout0 = MCREATE<int>(10);
    for (i = 0; i < IRout->len; i++) {
        int k = 0;
        for (int j = 0; j < IRout0->len; j++) {
            if (IRout->data[i] == IRout0->data[j]) {
                k = 1;
                break;
            }
        }
        if (k == 0) {
            MSET(*IRout0, IRout->data[i]);
            //cout << IRout->data[i] << ' ';
        }
    }

    MyArr<int>* IRout1 = MCREATE<int>(10);
    int lenIR = IRout0->len;
    for (i = 0; i < lenIR; i++) {
        int minIR = 1001;
        int indMin;
        for (int j = 0; j < IRout0->len; j++) {
            if (IRout0->data[j] < minIR) {
                minIR = IRout0->data[j];
                indMin = j;
            }
        }
        MSET(*IRout1, minIR);
        MDEL(*IRout0, indMin);
    }
    //cout << '\n';
    for (i = 0; i < IRout1->len; i++) {
        int IndexC = IRout1->data[i];
        //cout << IndexC << ' ';
        cout << setw(12) << left << Cross.data[IndexC].data[0] << Cross.data[IndexC].data[1] << '\n';
    }

    return 0;
}

