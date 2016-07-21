//
// Created by ÁõÊæÂü on 16/7/21.
//

#include "DataBase/DBConnPool.h"
#include <iostream>
using namespace std;

int main()
{
    cout << "DBConnPool Start..." << endl;

    if (!DBConnPool::GetInstance()->Initialize("easy_trans", "localhost", "root", "123456", 3306))
    {
        cout << "DBConnPool Start failed." << endl;
        throw -1;
    }
    cout << "DBConnPool Start Success." << endl;

    int x;
    cin >> x;

    return 1;
}

