#include "systemclass.h"

// i will initialize the object pointers to null to prevent error when 
// shutdown tries to clear them and they do not exist
SystemClass::SystemClass()
{
    m_Input = 0;
    m_Application = 0;
}

//now creating empty copy constructor and class destructor or my compiler might try
// to create them on its own and im not really sure how theyll be handled
SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
    int screenWidth, screenHeight;
    bool result;

    //binitialize height and width variables before sending to function
    screenWidth = 0;
    screenHeight = 0;

    //initialize windows api
    InitializeWindows(screenWidth, screenHeight);

    //create and initialize input object
    //use for reading keyboard
    m_Input = new InputClass;

    m_Input->Initialize();

    // make application class object
    // will be used to handle rendering graphics
    m_Application = new ApplicationClass;

    result = m_Application->Initialize(screenWidth, screenHeight, m_hwnd);

    // if initialization happened then returns true and if it didnt then returns false
    if(!result)
    {
        return false;
    }
    return true;
}

// shutdown function to clear memory and do cleanup and also destroy the window
void SystemClass::Shutdown()
{
    // to release the application object
    if(m_Application)
    {
        m_Application->Shutdown();
        delete m_Application;
        m_Application = 0;
    }

    //release input onject
    if(m_Input)
    {
        delete m_Input;
        m_Input = 0;
    }

    // finally destroy the window
    ShutdownWindows();

    return;
}


// Now a function that actually runs the program every frame in a loop until we quit it
// it should -check for windows system messages
//           -process those system messages
//           -do the application loop
//           -check if the user decided to quit while processing the frame
void SystemClass::Run()
{
    MSG msg;
    bool done, result;

    //initialize the message form structure
    ZeroMemory(&msg, sizeof(MSG));

    //loops until user inputs a quit message
    done = false;
    while(!done)
    {
        // handle the windows messages
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //check if windows tells to quit the application and do it
        if(msg.message == WM_QUIT)
        {
            done=true;
        }
        else
        {
            // else process the frame
            result = Frame();
            if(!result)
            {
                done = true;
            }
        }
    }
    return;
}

// now we process the actual frame
//chekc if user pressed esc key, if they did then quit,
// else call application object and process the frame
bool SystemClass::Frame()
{
    bool result;

    // check if escape key was pressed
    if(m_Input->IsKeyDown(VK_ESCAPE))
    {
        return false;
    }

    // process the frame
    result = m_Application->Frame();
    if(!result)
    {
        return false;
    }

    //since user wants to continue return true
    return true;
}

// now we can read windows messages to listen for key presses and releases
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch(umsg)
    {
        //check for key that was pressed
        case WM_KEYDOWN:
        {
            //if key was pressed then sending to input object to record it
            m_Input->KeyDown((unsigned int)wparam);
            return 0;
        }

        //check for key that was lifted
        case WM_KEYUP:
        {
            //if key was lifted then sending to input object to record it
            m_Input->KeyUp((unsigned int)wparam);
            return 0;
        }

        // to catch any other cases that windows may throw at us
        default:
        {
            return DefWindowProc(hwnd,umsg,wparam,lparam);
        }
    }
}


//initializewindows function to build the window we will send our render to
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
    WNDCLASSEX wc;
    DEVMODE dmScreenSettings;
    int posX, posY;

    // getting external pointer to this object
    ApplicationHandle = this;

    //getting instance of this application
    m_hinstance = GetModuleHandle(NULL);

    // NAMING IT
    m_applicationName = L"PHAGE";

    //setting up the window with default windows settings
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_applicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    //registering the window class
    RegisterClassEx(&wc);

    // get the resolution of your screen
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // setup screen settings based on fullscreen or windowed mode
    if(FULL_SCREEN)
    {
        // its true so set resolution to max of their screen that we got earlier
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
        dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // change the setting to full screen
        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
        posX = posY = 0;
    }
    else
    {
        // if windowed then set to half the screen size and center it
        screenWidth = (screenWidth / 2);
        screenHeight = (screenHeight / 2);
        posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
    }

    // creating the window with these screen settings
    // also getting the handle to it
    m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

    //focusing on the window upon launch
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);

    //hide cursor on this window
    ShowCursor(false);

    return;
}

// shutdownwindows function to set screen settings back to normal and
// releasee the window with all the handles
void SystemClass::ShutdownWindows()
{
    // show the mopuse cursor
    ShowCursor(true);

    // exit from full scren if that was chosen
    if(FULL_SCREEN)
    {
        ChangeDisplaySettings(NULL, 0);
    }

    //remove the window
    DestroyWindow(m_hwnd);
    m_hwnd = NULL;

    // remove the instance
    UnregisterClass(m_applicationName, m_hinstance);
    m_hinstance = NULL;

    // release the pointer
    ApplicationHandle = NULL;

    return;
}


// now we just make a wndproc function that windows sends its functions to
// we use this name in our input handler
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
    switch(umessage)
    {
        // check if the window is being destroyed
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        // check if the window is being closed
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        }

        // send all other messages to the message handler
        default:
        {
            return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
        }
    }
}

// and we're DONE