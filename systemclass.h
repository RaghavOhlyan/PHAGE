#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// preprocessor directive to only include the Windows headers we need
#define WIN32_LEAN_AND_MEAN

// including windows header to create and destroy windows
#include <windows.h>

// things in my class
#include "inputclass.h"
#include "applicationclass.h"

// defining my class SystemClass
class SystemClass
{
public:
    SystemClass();
    SystemClass(const SystemClass&);
    ~SystemClass();

    bool Initialize();
    void Shutdown();
    void Run();

    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
    bool Frame();
    void InitializeWindows(int&, int&);
    void ShutdownWindows();
private:
    LPCWSTR m_applicationName;
    HINSTANCE m_hinstance;
    HWND m_hwnd;

    InputClass* m_Input;
    ApplicationClass* m_Application;
};

// function prototype for the windows message handler
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global dec for my app object
static SystemClass* ApplicationHandle = 0;

#endif