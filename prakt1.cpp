#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <filesystem>
#include <stdlib.h>
using namespace std;
#include<windows.h>

class MyArray;
int Data(int& rows, int& cols, string& json, string& name, int& tuples_limit);
MyArray Array(int rows, int cols, string json);
int CreateDir(MyArray TableHeader, int rows, string name);
int CreateFol(MyArray TableHeader, int rows, string name, int arrayIndex, string numbers);
string SizeIndex(string str, int begin, int end);
int Lock1(string name, string table);
int Lock0(string name, string table);
int Insert(string str, MyArray TableHeader, int rows, string name, string& numbers);
int Delet(string str, MyArray TableHeader, int rows, string name, string numbers);
int Select(string str, MyArray TableHeader, int rows, string name, string numbers);
int Where(string str, MyArray TableHeader, int rows, string name, string numbers);
class MyArray {
private:
    string** array; // Указатель на указатель для двумерного массива
    int rows;            // Количество строк
    int cols;            // Количество столбцов

public:
    // Конструктор
    MyArray(int r, int c) : rows(r), cols(c) {
        // Выделяем память для строк
        array = new string * [rows];
        for (int i = 0; i < rows; ++i) {
            array[i] = new string[cols]; // Выделяем память для каждого столбца
        }
    }
    /*
    // Деструктор
    ~MyArray() {
        for (int i = 0; i < rows; ++i) {
            delete[] array[i]; // Освобождаем память для каждого столбца
        }
        delete[] array; // Освобождаем память для строк
    }
*/
    // Метод для установки значения
    void set(int r, int c, const string& value) {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            array[r][c] = value;
        }
        else {
            cerr << "Индексы вне диапазона!" << endl;
        }
    }

    // Метод для получения значения
    string get(int r, int c) const {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            return array[r][c];
        }
        else {
            cerr << "Индексы вне диапазона!" << endl;
            return "";
        }
    }

    // Метод для отображения массива
    void display() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << array[i][j] << "\t";
            }
            cout << endl;
        }
    }
};


int main() {

    setlocale(LC_ALL, "ru");

    string json;
    string name;
    int rows = 0;
    int cols = 0;
    int tuples_limit = 0;
    
    Data(rows, cols, json, name, tuples_limit);
    MyArray TableHeader = Array(rows, cols, json);

    string numbers;
    for (int i = 0; i < rows; i++) {
        numbers = numbers + '1';
    }

    CreateDir(TableHeader, rows, name);

    for (int i = 0; i < rows; i++) {
        CreateFol(TableHeader, rows, name, i, numbers);
    }
    
    cout << "СУБД готова к введению запросов \n";

    while (true)
    {
        string sql;
        getline(cin, sql);

        if (SizeIndex(sql, 0, 12) == "INSERT INTO ") {
            Insert(sql, TableHeader, rows, name, numbers);
        }
        else if (SizeIndex(sql, 0, 12) == "DELETE FROM ") {
            Delet(sql, TableHeader, rows, name, numbers);
        }
        else if (SizeIndex(sql, 0, 7) == "SELECT ") {
            Select(sql, TableHeader, rows, name, numbers);
        }
        else if (SizeIndex(sql, 0, 6) == "WHERE ") {
            Where(sql, TableHeader, rows, name, numbers);
        }
        else {
            cout << "Попробуйте ввести запрос ещё раз)";
        }
    }

    return 0;
}

int Data(int& rows, int& cols, string& json, string& name, int& tuples_limit) {

    string path = "schems.json";
    ifstream fin;
    fin.open(path);

    ;

    if (!fin.is_open())    // файл в строку
    {
        cout << "Ошибка открытия файла .json";
    }
    else {
        char ch;
        while (fin.get(ch))
        {
            json.push_back(ch);
        }
    }

    string tuples_limit_str;

    int schet_kov = 0;
    int schet_dvo = 0;
    int i = 0;
    while (true) {         // имя, лимит, размер двумерного массива

        if (schet_kov == 3 and json[i] != '"') { 
            name.push_back(json[i]);
        }
        if (json[i] == '"') {
            schet_kov++;
        }

        if (schet_dvo == 2 and (static_cast<int>(json[i]) > 47) and (static_cast<int>(json[i]) < 58)) {
            tuples_limit_str.push_back(json[i]);
        }
        if (json[i] == ':') {
            schet_dvo++;
        }

        if (json[i] == '[') {
            rows++;
            int cols0 = 0;
            while (json[i] != ']') {
                if (json[i] == ',') {
                    cols0++;
                }
                i++;
            }
            cols0++;
            cols = max(cols, cols0);
        }

        i++;
        if (json[i] == '}') {
            break;
        }
    }
    cols += 2;

    for (int k = 0; k < tuples_limit_str.size(); k++) { // преобразование в инт
        int degree = tuples_limit_str.size() - k - 1;
        tuples_limit += (static_cast<int>(tuples_limit_str[k]) - 48) * pow(double (10), double (degree));
    }

    return 0;
}
MyArray Array(int rows, int cols, string json) {

    MyArray TableHeader(rows, cols);

    int schet_scob = 0;   
    int i = 0;
    while (true) { // данные в двумерный массив

        if (json[i] == '{') {
            schet_scob++;
        }

        if (schet_scob == 2) {         
            for (int irows = 0; irows < rows; irows++) {
                int icols = 1;
                int sizeCols = 1;
                while (json[i] != ']') {
                    if (json[i] == '"') {
                        i++;
                        string word = "";
                        while (json[i] != '"') {
                            word.push_back(json[i]);
                            i++;
                        }
                        TableHeader.set(irows, icols, word);
                        sizeCols++;

                        icols++;
                    }
                    i++;
                    char siz = sizeCols;
                    string si;
                    si.push_back(siz);
                    TableHeader.set(irows, 0, si);
                }
                i++;
            }
            break;
        }
        i++;
    }
    return TableHeader;
}
int CreateDir(MyArray TableHeader, int rows, string name) {

    wstring A(name.begin(), name.end());

    const wstring basePath = L"C:\\abc\\Pract1\\";

    wstring fullPath = basePath;
    fullPath.append(A);

    if (CreateDirectory(fullPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {}

    else {
        wcerr << L"Ошибка при создании папки " << GetLastError() << endl;       
    }
    
    for (int i = 0; i < rows; i++) {

        string Dir = name + "\\" + TableHeader.get(i, 1);
        wstring A(Dir.begin(), Dir.end());
        const wstring basePath = L"C:\\abc\\Pract1\\";
        
        wstring fullPath = basePath;
        fullPath.append(A);

        if (CreateDirectory(fullPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {

            string namefile = "C:\\abc\\Pract1\\" + name + "\\" + TableHeader.get(i, 1) + "\\" + TableHeader.get(i, 1) + "_pk";
            ofstream fout;
            fout.open(namefile);
            if (fout.is_open()) {
                fout << "1";
            }
            fout.close();

            namefile = "C:\\abc\\Pract1\\" + name + "\\" + TableHeader.get(i, 1) + "\\" + TableHeader.get(i, 1) + "_lock";
            fout.open(namefile);
            if (fout.is_open()) {
                fout << "0";
            }
            fout.close();

        }
        else {
            wcerr << L"Ошибка при создании папки " << GetLastError() << endl;
        }
    }
    return 0;
}
int CreateFol(MyArray TableHeader, int rows, string name, int arrayIndex, string numbers) {

    char number = numbers[arrayIndex];
    string namefile = "C:\\abc\\Pract1\\" + name + "\\" + TableHeader.get(arrayIndex, 1) + "\\" + number + ".csv";
    ofstream fout;
    fout.open(namefile);
    if (fout.is_open()) {

        for (int i = 2; i < (TableHeader.get(arrayIndex, 0)[0]); i++) {
            fout << "," << TableHeader.get(arrayIndex, i);
        }
        fout << "\n";
    }
    fout.close();

    return 0;
}
string SizeIndex(string str, int begin, int end) {
    string strcut;
    for (int i = begin; i < end; i++) {
        strcut.push_back(str[i]);
    }
    return strcut;
}
int Lock1(string name, string table) {

    string namefile = "C:\\abc\\Pract1\\" + name + "\\" + table + "\\" + table + "_lock";
    string temp;
    ifstream some_file(namefile);
    some_file >> temp;
    some_file.close();
    int tempint = atoi(temp.c_str());
    ofstream fout;
    fout.open(namefile);
    if (fout.is_open()) {
        fout << 1;
    }
    fout.close();

    return 0;
}
int Lock0(string name, string table) {

    string namefile = "C:\\abc\\Pract1\\" + name + "\\" + table + "\\" + table + "_lock";
    string temp;
    ifstream some_file(namefile);
    some_file >> temp;
    some_file.close();
    int tempint = atoi(temp.c_str());
    ofstream fout;
    fout.open(namefile);
    if (fout.is_open()) {
        fout << 0;
    }
    fout.close();

    return 0;
}
int Insert(string str, MyArray TableHeader, int rows, string name, string& numbers) {
    
    string table;
    int i = 12;
    while (str[i] != ' ') {
        table.push_back(str[i]);
        i++;
    }
    
    if (i >= str.size() or SizeIndex(str, i + 1, i + 7) != "VALUES") {
        cout << "Попробуйте ввести запрос ещё раз)";
        return 0;
    }
    
    char number;
    int tableHere = 0;
    int j;
    for (j = 0; j < rows; j++) {
        if (TableHeader.get(j, 1) == table) {
            tableHere = 1;
            number = numbers[j];
            break;
        }
    }
    
    if (tableHere == 0) {
        cout << "Такой таблицы нет";
        return 0;
    }
    Lock1(name, table);
   
    string instr;
    int kol = 1;
    i = i + 7;
    while (i != str.size()) {
        if ((str[i] != ' ') and (str[i] != '\'') and (str[i] != '(') and (str[i] != ')')) {
            instr.push_back(str[i]);
        }       
        if (str[i] == ',') {
            kol++;
        }
        i++;
    }

    int kolrite = (TableHeader.get(j, 0)[0]);
    if (kol != (kolrite - 2)) {
        cout << "Неверное колличество элементов";
        return 0;
    }

    string namefile = "C:\\abc\\Pract1\\" + name + "\\" + table + "\\" + table + "_pk";
    string temp;
    ifstream some_file(namefile); 
    some_file >> temp;
    some_file.close(); 
    int tempint = atoi(temp.c_str());
    ofstream fout;
    fout.open(namefile);
    if (fout.is_open()) {
        fout << tempint + 1;
    }
    fout.close();
    if (tempint > 1000) {
        numbers[j]++;
        CreateFol(TableHeader, rows, name, j, numbers);
    }

    instr = temp + "," + instr + "\n";
    namefile = "C:\\abc\\Pract1\\" + name + "\\" + table + "\\" + number + ".csv";
    ofstream file(namefile, ios_base::app);
    if (file.is_open()) { 
        file << instr; 
        file.close(); 
    }
    Lock0(name, table);

    return 0;
}
int Delet(string str, MyArray TableHeader, int rows, string name, string numbers) {

    string table;
    int i = 12;
    while (i != str.size()) {
        table.push_back(str[i]);
        i++;
    }

    char number;
    int tableHere = 0;
    int j;
    for (j = 0; j < rows; j++) {
        if (TableHeader.get(j, 1) == table) {
            tableHere = 1;
            number = numbers[j];
            break;
        }
    }

    if (tableHere == 0) {
        cout << "Такой таблицы нет";
        return 0;
    }
    Lock1(name, table);

    string sql;
    getline(cin, sql);

    if (i >= sql.size() or SizeIndex(sql, 0, 6) != "WHERE ") {
        cout << "Попробуйте ввести запрос ещё раз)";
        return 0;
    }

    i = 6;
    string table0;
    while (sql[i] != '.') {
        table0.push_back(sql[i]);
        i++;
    }
    if (table0 != table) {
        cout << "Попробуйте ввести запрос ещё раз)";
        return 0;
    }

    string kol;
    i++;
    while (sql[i] != ' ') {
        kol.push_back(sql[i]);
        i++;
    }
    int kolhere = 0;
    for (int k = 2; k < (TableHeader.get(j, 0)[0]); k++) {
        if (TableHeader.get(j, k) == kol) {
            kolhere = k;
        }
    }
    if (kolhere == 0) {
        cout << "Такой колонки нет";
        return 0;
    }

    string instr;
    while (i < sql.size()) {
        if ((sql[i] != ' ') and (sql[i] != '\'') and (sql[i] != '=')) {
            instr.push_back(sql[i]);
        }
        i++;
    }



    string temp;
    string namefile = "C:\\abc\\Pract1\\" + name + "\\" + table + "\\" + number + ".csv";
    ifstream fin;
    fin.open(namefile);

    if (!fin.is_open())    // файл в строку
    {
        cout << "Ошибка открытия файла ";
    }
    else {
        char ch;
        while (fin.get(ch))
        {
            temp.push_back(ch);
        }
    }



    string intemp;
    for (int i = 2; i < (TableHeader.get(j, 0)[0]); i++) {
        intemp.push_back(','); 
        intemp += (TableHeader.get(j, i));
    }
    intemp.push_back('\n');
 
    i = 0;
    while (i < temp.size()) {
        int all = 0;
        if (temp[i] == '\n') {
            i++;
            if (i >= temp.size()) {
                all = 1;
                break;
            }
            int istart = i;
            int iend;
            int zap = 1;
            string strdel;
            while (true) {
                if (temp[i] == ',') {
                    zap++;
                    i++;
                }
                if (kolhere == zap) {
                    strdel.push_back(temp[i]);
                }
                if (temp[i] == '\n') {
                    iend = i + 1;
                    i -= 1;

                    if (strdel != instr) {
                        intemp += SizeIndex(temp, istart, iend);
                    }

                    break;
                }
                i++;
            }
        }
        if (all == 1) {
            break;
        }
        i++;
    }

    int tempint = atoi(temp.c_str());
    ofstream fout;
    fout.open(namefile);
    if (fout.is_open()) {
        fout << intemp;
    }
    fout.close();

    Lock0(name, table);

    return 0;
}
int Select(string str, MyArray TableHeader, int rows, string name, string numbers) {   
    int sizeSel = 1;
    int i = 7;
    while (true) {
        if (i >= str.size()) {
            break;
        }
        if (str[i] == ',') {
            sizeSel++;
        }
        i++;
    }

    MyArray Sel(sizeSel, 2);

    i = 7;
    string takl = "";
    int numRows = 0;
    while (true) {
        if (i >= str.size()) {
            int k = 0;
            string word = "";
            for (int j = 0; j < takl.size(); j++) {
                if (takl[j] == '.') {
                    Sel.set(numRows, k, word);
                    word.clear();
                    k = 1;
                }
                else {
                    word.push_back(takl[j]);
                }
            }
            Sel.set(numRows, k, word);
            break;
        }
        if (str[i] == ',') {
            int k = 0;
            string word;
            for (int j = 0; j < takl.size(); j++) {
                if (takl[j] == '.') {
                    Sel.set(numRows, k, word);
                    word.clear();
                    k = 1;
                }
                else {
                    word.push_back(takl[j]);
                }
            }
            takl = "";
            Sel.set(numRows, k, word);
            numRows++;
        }
        else if (str[i] != ' ') {
            takl.push_back(str[i]);
        }
        i++;
    }

    int datetrue = 0;
    for (i = 0; i < sizeSel; i++) {
        for (int j = 0; j < rows; j++) {
            if (Sel.get(i, 0) == TableHeader.get(j, 1)) {
                for (int k = 2; (k < (TableHeader.get(j, 0)[0])); k++) {
                    if (Sel.get(i, 1) == TableHeader.get(j, k)) {
                        datetrue++;
                    }
                }
            }
        }
    }

    if (datetrue != sizeSel) {
        cout << "Данные некоректны";
        return 0;
    }

    string sql;
    getline(cin, sql);

    if (i >= sql.size() or SizeIndex(sql, 0, 5) != "FROM ") {
        cout << "Попробуйте ввести запрос ещё раз)";
        return 0;
    }

    i = 5;
    numRows = 0;
    string word;
    while (true) {
        if (i >= sql.size()) {
            if (Sel.get(numRows, 0) != word) {
                cout << "Данные некоректны";
                return 0;
            }
            break;
        }
        if (sql[i] == ',') {
            if (Sel.get(numRows, 0) != word) {
                cout << "Данные некоректны";
                return 0;
            }
            numRows++;
            word.clear();
        }
        else if (sql[i] != ' ') {
            word.push_back(sql[i]);
        }
        i++;
    }

    int kolstr = 1;
    for (i = 0; i < sizeSel; i++) {
        char number;
        int j = 0;
        for (j = 0; j < rows; j++) {
            if (TableHeader.get(j, 1) == Sel.get(i, 0)) {
                number = numbers[j];
                break;
            }
        }

        string temp;
        string namefile = "C:\\abc\\Pract1\\" + name + "\\" + Sel.get(i, 0) + "\\" + number + ".csv";
        ifstream fin;
        fin.open(namefile);
        if (!fin.is_open())    // файл в строку
        {
            cout << "Ошибка открытия файла ";
        }
        else {
            char ch;
            while (fin.get(ch))
            {
                temp.push_back(ch);
            }
        }
        ofstream fout;
        fout.open(namefile);
        if (fout.is_open()) {
            fout << temp;
        }
        fout.close();

        int mnoj = 0;
        for (int k = 0; k < temp.size(); k++) {
            if (temp[k] == '\n') {
                mnoj++;
            }
        }
        mnoj--;

        kolstr = kolstr * mnoj;
    }

    MyArray Cross(kolstr, (sizeSel * 2));

    for (i = 0; i < sizeSel; i++) {
        char number;
        int j = 0;
        for (j = 0; j < rows; j++) {
            if (TableHeader.get(j, 1) == Sel.get(i, 0)) {
                number = numbers[j];
                break;
            }
        }

        string temp;
        string namefile = "C:\\abc\\Pract1\\" + name + "\\" + Sel.get(i, 0) + "\\" + number + ".csv";
        ifstream fin;
        fin.open(namefile);
        if (!fin.is_open())    // файл в строку
        {
            cout << "Ошибка открытия файла ";
        }
        else {
            char ch;
            while (fin.get(ch))
            {
                temp.push_back(ch);
            }
        }
        ofstream fout;
        fout.open(namefile);
        if (fout.is_open()) {
            fout << temp;
        }
        fout.close();

        int tabnum = 0;
        for (int k = 0; k < rows; k++) {
            if (Sel.get(i, 0) == TableHeader.get(k, 1)) {
                tabnum = k;
            }
        }

        int kolnum = 0;
        for (int k = 2; k < TableHeader.get(tabnum, 0)[0]; k++) {
            if (Sel.get(i, 1) == TableHeader.get(tabnum, k)) {
                kolnum = k - 1;
            }
        }
      
        int u = 0;
        while (u < kolstr) {
            int k = 0;
            while (k < (temp.size() - 1)) {
                if (temp[k] == '\n') {
                    k++;
                    int zap = 0;
                    string index;
                    string word;
                    while (temp[k] != '\n') {
                        if (temp[k] == ',') {
                            zap++;
                            k++;
                        }
                        if (zap == 0) {
                            index.push_back(temp[k]);
                        }
                        if (zap == kolnum) {
                            word.push_back(temp[k]);
                        }
                        k++;
                    }
                        Cross.set((u), (i * 2), index);
                        Cross.set((u), (i * 2 + 1), word);
                    }
                    word.clear();
                    index.clear();
                    u++;
                    k--;
                }
                k++;
            }
        }
    }

    cout << "Cross join\n";
    Cross.display();

    return 0;
}
int Where(string str, MyArray TableHeader, int rows, string name, string numbers) {
    int i = 6;
    string word;
    int kolus = 6;

    while (i < str.size()) {
        if (str[i] == '=') {
            kolus--;
        }
        i++;
    }

    MyArray Usl(2, 8);
    i = 6;
    if (kolus == 0) {
        int usl = 0;
        while (i < str.size()) {
            if (str[i] != ' ') {
                word.push_back(str[i]);
            }
            else {
                Usl.set(usl, 0, word);
                usl = 1;
                word.clear();
                if (str[i + 1] != '=' or str[i + 2] != ' ') {
                    cout << "Попробуйте ввести запрос ещё раз)";
                    return 0;
                }
                i += 2;
            }
            i++;
        }

        if (Usl.get(0, 0)[0] == '\'' and Usl.get(1, 0)[0] == '\'') {
            cout << "Попробуйте ввести запрос ещё раз)";
            return 0;
        }
        if (Usl.get(0, 0)[0] == '\'') {
            string zam = Usl.get(0, 0);
            Usl.get(0, 0) = Usl.get(1, 0);
            Usl.get(1, 0) = zam;
        }
    }

    if (kolus == 1) {
        int usl = 0;
        while (i < str.size()) {
            if (str[i] != ' ') {
                word.push_back(str[i]);
            }
            else {
                Usl.set(usl, 0, word);
                usl = 1;
                word.clear();
                if (str[i + 1] != '=' or str[i + 2] != ' ') {
                    cout << "Попробуйте ввести запрос ещё раз)";
                    return 0;
                }
                i += 2;
            }
            i++;
        }

        if (Usl.get(0, 0)[0] == '\'' and Usl.get(1, 0)[0] == '\'') {
            cout << "Попробуйте ввести запрос ещё раз)";
            return 0;
        }
        if (Usl.get(0, 0)[0] == '\'') {
            string zam = Usl.get(0, 0);
            Usl.get(0, 0) = Usl.get(1, 0);
            Usl.get(1, 0) = zam;
        }

        if (Usl.get(1, 0)[0] == '\'') {
            int folk = 0;
            string table, kolmn;
            int k = 0;
            while (k < Usl.get(0, 0).size()) {
                if (Usl.get(0, 0)[k] == '.') {
                    folk = 1;
                    k++;
                }
                if (folk == 0) {
                    table.push_back(Usl.get(0, 0)[k]);
                }
                if (folk == 0) {
                    kolmn.push_back(Usl.get(0, 0)[k]);
                }
                k++;
            }

            char number;
            int j = 0;
            for (j = 0; j < rows; j++) {
                if (TableHeader.get(j, 1) == TableHeader.get(i, 1)) {
                    number = numbers[j];
                    break;
                }
            }
            string temp;
            string namefile = "C:\\abc\\Pract1\\" + name + "\\" + TableHeader.get(i, 1) + "\\" + number + ".csv";
            ifstream fin;
            fin.open(namefile);
            if (!fin.is_open())    // файл в строку
            {
                cout << "Ошибка открытия файла ";
            }
            else {
                char ch;
                while (fin.get(ch))
                {
                    temp.push_back(ch);
                }
            }
            ofstream fout;
            fout.open(namefile);
            if (fout.is_open()) {
                fout << temp;
            }
            fout.close();

            string instr = SizeIndex(Usl.get(1, 0), 1, Usl.get(1, 0).size() - 1);

            int kolhere = 0;

            for (int k = 2; k < (TableHeader.get(j, 0)[0]); k++) {
                if (TableHeader.get(j, k) == kolmn) {
                    kolhere = k;
                }
            }
            if (kolhere == 0) {
                cout << "Такой колонки нет";
                return 0;
            }

            string intemp;
            while (i < temp.size()) {
                int all = 0;
                if (temp[i] == '\n') {
                    i++;
                    if (i >= temp.size()) {
                        all = 1;
                        break;
                    }
                    int istart = i;
                    int iend;
                    int zap = 1;
                    string strdel;
                    while (true) {
                        if (temp[i] == ',') {
                            zap++;
                            i++;
                        }
                        if (kolhere == zap) {
                            strdel.push_back(temp[i]);
                        }
                        if (temp[i] == '\n') {
                            iend = i + 1;
                            i -= 1;

                            if (strdel == instr) {
                                intemp += SizeIndex(temp, istart, iend);
                            }

                            break;
                        }
                        i++;
                    }
                }
                i++;
            }
        }
    }
    if (kolus == 1) {
        int usl = 0;
        while (i < str.size()) {
            if (str[i] != ' ') {
                word.push_back(str[i]);
            }
            else {
                Usl.set(usl, 0, word);
                usl = 1;
                word.clear();
                if (str[i + 1] != '=' or str[i + 2] != ' ') {
                    cout << "Попробуйте ввести запрос ещё раз)";
                    return 0;
                }
                i += 2;
            }
            i++;
        }

        if (Usl.get(0, 0)[0] == '\'' and Usl.get(1, 0)[0] == '\'') {
            cout << "Попробуйте ввести запрос ещё раз)";
            return 0;
        }
        if (Usl.get(0, 0)[0] == '\'') {
            string zam = Usl.get(0, 0);
            Usl.get(0, 0) = Usl.get(1, 0);
            Usl.get(1, 0) = zam;
        }

        if (Usl.get(1, 0)[0] == '\'') {
            int folk = 0;
            string table, kolmn;
            int k = 0;
            while (k < Usl.get(0, 0).size()) {
                if (Usl.get(0, 0)[k] == '.') {
                    folk = 1;
                    k++;
                }
                if (folk == 0) {
                    table.push_back(Usl.get(0, 0)[k]);
                }
                if (folk == 0) {
                    kolmn.push_back(Usl.get(0, 0)[k]);
                }
                k++;
            }

            char number;
            int j = 0;
            for (j = 0; j < rows; j++) {
                if (TableHeader.get(j, 1) == TableHeader.get(i, 1)) {
                    number = numbers[j];
                    break;
                }
            }
            string temp;
            string namefile = "C:\\abc\\Pract1\\" + name + "\\" + TableHeader.get(i, 1) + "\\" + number + ".csv";
            ifstream fin;
            fin.open(namefile);
            if (!fin.is_open())    // файл в строку
            {
                cout << "Ошибка открытия файла ";
            }
            else {
                char ch;
                while (fin.get(ch))
                {
                    temp.push_back(ch);
                }
            }
            ofstream fout;
            fout.open(namefile);
            if (fout.is_open()) {
                fout << temp;
            }
            fout.close();

            string instr = SizeIndex(Usl.get(1, 0), 1, Usl.get(1, 0).size() - 1);

            int kolhere = 0;

            for (int k = 2; k < (TableHeader.get(j, 0)[0]); k++) {
                if (TableHeader.get(j, k) == kolmn) {
                    kolhere = k;
                }
            }
            if (kolhere == 0) {
                cout << "Такой колонки нет";
                return 0;
            }

            string intemp;
            while (i < temp.size()) {
                int all = 0;
                if (temp[i] == '\n') {
                    i++;
                    if (i >= temp.size()) {
                        all = 1;
                        break;
                    }
                    int istart = i;
                    int iend;
                    int zap = 1;
                    string strdel;
                    while (true) {
                        if (temp[i] == ',') {
                            zap++;
                            i++;
                        }
                        if (kolhere == zap) {
                            strdel.push_back(temp[i]);
                        }
                        if (temp[i] == '\n') {
                            iend = i + 1;
                            i -= 1;

                            if (strdel == instr) {
                                intemp += SizeIndex(temp, istart, iend);
                            }

                            break;
                        }
                        i++;
                    }
                }
                i++;
            }
        }
    }
    if (str.size() != 33) {
        cout << "Такой колонки нет";
        return 0;
    }
    
    if (kolus == 0) {
        int usl = 0;
        while (i < str.size()) {
            if (str[i] != ' ') {
                word.push_back(str[i]);
            }
            else {
                Usl.set(usl, 0, word);
                usl = 1;
                word.clear();
                if (str[i + 1] != '=' or str[i + 2] != ' ') {
                    cout << "Попробуйте ввести запрос ещё раз)";
                    return 0;
                }
                i += 2;
            }
            i++;
        }

        if (Usl.get(0, 0)[0] == '\'' and Usl.get(1, 0)[0] == '\'') {
            cout << "Попробуйте ввести запрос ещё раз)";
            return 0;
        }
        if (Usl.get(0, 0)[0] == '\'') {
            string zam = Usl.get(0, 0);
            Usl.get(0, 0) = Usl.get(1, 0);
            Usl.get(1, 0) = zam;
        }
    }

    cout << "Выборка:\n";
    Usl.display();

    return 0;
}






