#include <iostream>
#include <fstream>

#include "hash.h"
#include "arr.h"


using namespace std;



string SizeIndex(string str, int begin, int end) {
    if (str.size() < end) {
        cout << "Попробуйте ввести запрос ещё раз)";
        return 0;
    }
    string strcut;
    for (int i = begin; i < end; i++) {
        strcut.push_back(str[i]);
    }
    return strcut;
}



MyArr<MyArr<string>> ReadCSV(string filename) {
    MyArr<MyArr<string>>* data = MCREATE<MyArr<string>>(10);
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
    }

    string line;
    getline(file, line);
    while (getline(file, line)) {
        MyArr<string>* row = MCREATE<string>(10);
        stringstream lineStream(line);
        string cell;

        while (getline(lineStream, cell, ',')) {
            MSET(*row, cell);
        }

        MSET(*data, *row);
    }

    file.close();
    return *data;
}




int AddInFile(string filename, string addstr) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 0;
    }

    string line;
    string allstr;
    while (getline(file, line)) {
        allstr += line;
        allstr.push_back('\n');
    }
    file.close();
    allstr += addstr;

    ofstream file2(filename);
    file2 << allstr;
    file2.close();
  
    return 0;
}
