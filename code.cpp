#include <iostream>
#include <fstream>

#include "hash.h"
#include "arr.h"

#include "data.h"
#include "select_.h"

using namespace std;

int main()
{   
    string fileName = "schems.json";
    string schemaName = "";
    int tuplesLimit = 0;
    MyHash<string, MyArr<string>*>* jsonStructure = HCREATE<string, MyArr<string>*>(10);
    MyArr<string>* keys = MCREATE<string>(10);

    ReadJsonFile(fileName, schemaName, tuplesLimit, *jsonStructure, *keys);

    MyHash<string, int>* numb = HCREATE<string, int>(10);
    for (int i = 0; i < keys->len; i++) {
        HSET<string, int>(*numb, keys->data[i], 1);
    }


cout << "СУБД готова к введению запросов, чтобы закончить нажмите Enter \n";

    while (true)
    {
        string sql;
        getline(cin, sql);

        if (SizeIndex(sql, 0, 12) == "INSERT INTO ") {
            Insert(sql, *jsonStructure, *numb, schemaName, tuplesLimit, *keys);
        }
        else if (SizeIndex(sql, 0, 12) == "DELETE FROM ") {
            Delet(sql, *jsonStructure, *numb, schemaName, *keys);
        }
        else if (SizeIndex(sql, 0, 7) == "SELECT ") {
            Select(sql, *jsonStructure, *numb, schemaName, *keys);
        }
        else if (sql.size() == 0) {
            break;
        }
        else {
            cout << "Попробуйте ввести запрос ещё раз)";
        }
    }
}
