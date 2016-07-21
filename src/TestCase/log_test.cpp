//
// Created by ÁõÊæÂü on 16/7/21.
//

#include "Log/LogPrinter.h"
#include "Log/LogStream.h"

int main()
{
    if (!LogPrinter::GetInstance() -> Start())
    {
        cout << "LogPrinter init failed. " << endl;
        exit(1);
    }

    cout << "log success." << endl;

    string x;

    while (cin >> x)
    {
        lout << x << endl;
    }

    return 0;
}

