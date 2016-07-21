//
// Created by ÁõÊæÂü on 16/7/21.
//

#include "EventEngine/EventEngine.h"

#include <iostream>
using namespace std;

class TestEngine
        : public EventEngine
{
private:
    void on_event(Event & e)
    {
        cout << "Get event : " << e._priority << "    Event_type : " << (*(e._p_edata)).GetType() << endl;
    }
};

class TestData
        : public EventData
{
public:
    const EDType GetType() const
    {
        return EDTYPE_LISTEN_NET_RES;
    }
};

int main()
{
    TestEngine engine;
    engine.Start();

    cout << "Start success. " << endl;

    TestData data;
    Event e(&data);
    //cout << (*(e._p_edata)).GetType();

    cout << "Begin";
    int x;
    cin >> x;
    engine.PostEvent(e);

    cout << "End";
    cin >> x;
    return 0;
}