#include "D:/PHAGE/systemclass.h"

int WinAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCMDShow)
{
    //Initializing the system class object and result boolean value
    SystemClass* System;
    bool result;

    // Create the system object
    System = new SystemClass;

    // Initialize and run the system object
    result = System->Initialize();
    // running the system object if initialize returns true
    if (result)
    {
        System->Run();
    }
    // Shutdown and release the system object
    System->Shutdown();
    delete System;
    System = 0;

    return 0;
}