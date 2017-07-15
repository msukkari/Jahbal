#include <windows.h>

#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd)
{
    Engine::GetInstance()->Init();

    Engine::GetInstance()->Run();
    return 0;
}