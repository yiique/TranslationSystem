//
// Created by ¡ı Ê¬¸ on 16/7/21.
//

#include "Configurator/Configurator.h"
#include "Log/LogPrinter.h"

#include <iostream>
#include <string>
using namespace std;


int main()
{
    LogPrinter::GetInstance()->Start();

    Configurator config;
    string config_path = "server_config.txt";

    if (!config.ReadConfig(config_path.c_str()))
    {
        cout << "Read config failed." << endl;
        throw -1;
    }

    cout << "Read config success." << endl;

    string x;
    cin >> x;

    return 1;
}