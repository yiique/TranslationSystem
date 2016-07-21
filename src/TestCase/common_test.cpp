//
// Created by ÁõÊæÂü on 16/7/21.
//

#include "Common/f_utility.h"

#include <iostream>

int main()
{
    cout << "Test fun count_words." << endl;

    string a = " a  bc edeaf sg  ";
    //cin >> a;

    string b = del_head_tail_blank(a);
    string c = filter_head_tail(a);

    cout << "String: " << a << "    Words: " << count_words(a) << endl;

    cout << "String: " << a << "    del_head_tail_blank: " << b << endl;

    cout << "String: " << a << "    filter_head_tail: " << c << endl;

    int x;
    cin >> x;

    return 0;

}