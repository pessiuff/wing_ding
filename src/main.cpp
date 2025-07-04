#include "game/game.h"

int main()
{
    wing_ding::game game {};
    return game.run();
}

#ifdef _WIN32
#ifdef NO_CONSOLE_WINDOW
#include <Windows.h>  
#include <sal.h>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)  
{  
    return main();  
}  
#endif
#endif