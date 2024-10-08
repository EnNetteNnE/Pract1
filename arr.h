#ifndef ARR
#define ARR

#include <iostream>
#include <iomanip>

using namespace std;

template <typename T>
struct MyArr {
    T* data;    
    int len;        
    int bulk;        
    int percent; 
};

template <typename T>    // создание
MyArr<T>* MCREATE(int iBulk) {
    if (iBulk <= 0) {    
        throw runtime_error("Index out of range");
    }
    int ipercent = 75;
    MyArr<T>* arr = new MyArr<T>;
    arr->data = new T[iBulk];
    arr->len = 0;
    arr->bulk = iBulk;
    arr->percent = ipercent;
    return arr;
}

template <typename T>     // увеличение массива
void EXPAND(MyArr<T>& arr) {
    int newBulk = arr.bulk * 2;
    T* newData = new T[newBulk];
    for (int i = 0; i < arr.len; i++) {     //копируем данные из старого массива в новый
        newData[i] = arr.data[i];
    }
    delete[] arr.data;                      // очистка памяти
    arr.data = newData;
    arr.bulk = newBulk;
}

template <typename T>    // добавление
void MSET(MyArr<T>& arr, T value) {
    if ((arr.len + 1) * 100 / arr.bulk >= arr.percent) { //обновление размера 
        EXPAND(arr);
    }
    arr.data[arr.len] = value;
    arr.len++;
}

template <typename T>     //удаление
void MDEL(MyArr<T>& arr, int index) {
    if (index < 0 || index >= arr.len) {
        throw runtime_error("Index out of range");
    }

    for (int i = index; i < arr.len - 1; i++) {
        arr.data[i] = arr.data[i + 1];
    }

    arr.len--;
}

template <typename T>    // замена
void MREPLACE(MyArr<T>& arr, int index, T value) {
    if (index < 0 || index >= arr.len) {
        throw runtime_error("Index out of range");
    }
    arr.data[index] = value;
}

template <typename T>
ostream& operator << (ostream& os, const MyArr<T>& arr) {
    for (int i = 0; i < arr.len; i++) {
        cout << arr.data[i];
        if (i < arr.len - 1) cout << "\t";
    }
    return os;
}

#endif
