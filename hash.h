#ifndef HASH
#define HASH

#include <iostream>
#include <string>

using namespace std;

template <typename string, typename V>
struct Nod {
    string key;
    V value;
    Nod* next;
};

template <typename string, typename V>
struct MyHash {
    Nod<string, V>** data;
    int len;
    int bulk;
    int percent;
};

template <typename string>   // хэшF
int djb2Hash(string key0) {
    int hash = 5381;
    char key[] = "key";
    for (char ch : key) {
        hash = ((hash << 5) + hash) + ch; /* hash * 33 + c */
    }
    return hash;
}

template <typename string, typename V>  // создвние
MyHash<string, V>* HCREATE(int iBulk) {
    if (iBulk <= 0) {
        throw runtime_error("Index out of range");
    }

    int ipercent = 75;
    MyHash<string, V>* map = new MyHash<string, V>;
    map->data = new Nod<string, V>*[iBulk]; 
    for (int i = 0; i < iBulk; i++) {
        map->data[i] = nullptr;
    }

    map->len = 0;
    map->bulk = iBulk;
    map->percent = ipercent;
    return map;
}

template <typename string, typename V>  // расширение
void EXPAND(MyHash<string, V>& map) {
    int newBulk = map.bulk * 2; //   при передаче по ссылке нужна точке!!!!               
    Nod<string, V>** newData = new Nod<string, V>*[newBulk];
    for (int i = 0; i < newBulk; i++) {
        newData[i] = nullptr;
    }    
    for (int i = 0; i < map.bulk; i++) {  // проход по всем ячейкам
        Nod<string, V>* gsd = map.data[i]; //  итая нода     
        while (gsd != nullptr) {   // проход и обновление         
            Nod<string, V>* next = gsd->next;  // новая нода с указателем на нехт
            int index = djb2Hash(gsd->key) % newBulk; // перехеш
            if (newData[index] != nullptr) {
                Nod<string, V>* smtime = newData[index];
                while (smtime->next != nullptr) {
                    smtime = smtime->next;
                }
                gsd->next = nullptr;
                smtime->next = gsd;
            }
            else {
                gsd->next = newData[index];
                newData[index] = gsd;
                gsd = next;
            }          
        }
    }
    delete[] map.data;
    map.data = newData;
    map.bulk = newBulk;
}

template <typename string, typename V>  // обработка коллизий
void COLLISION(MyHash<string, V>& map, int index, const string& key, const V& value) {
    Nod<string, V>* newNod = new Nod<string, V>{key, value, nullptr}; // создаем новую ноду со значениями
    Nod<string, V>* gsd = map.data[index];
    while (gsd->next != nullptr) {
        gsd = gsd->next;
    }
    gsd->next = newNod;
}

template <typename string, typename V>  // добавление
void HSET(MyHash<string, V>& map, const string& key, const V& value) {
    if ((map.len + 1) * 100 / map.bulk >= map.percent) {
        EXPAND(map);
    }
    int index = djb2Hash(key) % map.bulk;
    Nod<string, V>* temp = map.data[index];
    if (temp != nullptr) {
        if (temp->key == key) {            
            temp->value = value;// обновить 
            map.data[index] = temp;
        } else {
            COLLISION(map, index, key, value);
        }
    } else {
        Nod<string, V>* newNod = new Nod<string, V>{key, value, map.data[index]};
        map.data[index] = newNod;
        map.len++;
    }
}

template <typename string, typename V> // чтение
V HGET(const MyHash<string, V>& map, const string& key) {
    int index = djb2Hash(key) % map.bulk;
    Nod<string, V>* gsd = map.data[index];
    while (gsd != nullptr) {
        if (gsd->key == key) {
            return gsd->value;
        }
        gsd = gsd->next;
    }
    throw runtime_error("Key not found");
}

template <typename string, typename V>  // удаление 
void HDEL(MyHash<string, V>& map, const string& key) {
    int index = djb2Hash(key) % map.bulk;
    Nod<string, V>* gsd = map.data[index];
    Nod<string, V>* prev = nullptr;
    while (gsd != nullptr) {
        if (gsd->key == key) {
            if (prev == nullptr) {
                map.data[index] = gsd->next;
            } else {
                prev->next = gsd->next;
            }
            delete gsd;
            map.len--;
            return;
        }
        prev = gsd;
        gsd = gsd->next;
    }
    throw runtime_error("Key not found");
}

template <typename string, typename V>   // очистка 
void HCLEAN(MyHash<string, V>& map) {
    for (int i = 0; i < map.bulk; i++) {
        Nod<string, V>* gsd = map.data[i];
        while (gsd != nullptr) {
            Nod<string, V>* next = gsd->next;
            delete gsd;
            gsd = next;
        }
    }
    delete[] map.data;
    map.data = nullptr;
    map.len = 0;
    map.bulk = 0;
}

#endif
