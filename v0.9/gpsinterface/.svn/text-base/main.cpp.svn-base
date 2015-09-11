#include "gpsinterface.h"
#include <iostream>

int main(int argc, char **argv)
{
    vector<unsigned char> tuid;
   
    getElectronicFenceByType(FENCE_TYPE_CIRCLE, 0, NULL);
 
    if(InnovReadTuid(tuid) < 0)
    {
        cerr << "Failed to read tuid." << endl;
        return -1;
    }
    
    for(int i = 0; i < tuid.size(); i ++)
    {
        char ch = tuid[i];
        cout << ch;
    }

    cout << endl;
    return 0;
}
