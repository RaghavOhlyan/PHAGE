#include "inputclass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
    int i;

    // initialize all the keys to being lifted
    for(i=0; i<256; i++)
    {
        m_keys[i] = false;
    }

    return;
}

void InputClass::KeyDown(unsigned int input)
{
    // if a key is pressed then save that state in an array
    m_keys[input] = true;
    return;
}

void InputClass::KeyUp(unsigned int input)
{
    // when a key is released then remove it ffrom the array
    m_keys[input] = false;
    return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
    // return the state of a key, pressed/lifted
    return m_keys[key];
}