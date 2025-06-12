#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_CLASS "ApolloRuntimeContentWindow"

HHOOK g_mouse_hook = NULL;
bool g_is_running = false;
HWND g_locked_window = NULL;
bool g_is_clipped = false;
HWND g_hwnd_button;
HWND g_hwnd_label;

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && g_is_running)
    {
        if (g_locked_window == NULL)
        {
            g_locked_window = FindWindowA(WINDOW_CLASS, NULL);
        }

        HWND current_window = GetForegroundWindow();

        if (!g_locked_window || current_window != g_locked_window)
        {
            if (g_is_clipped)
            {
                ClipCursor(NULL);
                g_is_clipped = false;
            }
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        RECT rect;
        if (GetWindowRect(g_locked_window, &rect))
        {
            if (!g_is_clipped)
            {
                ClipCursor(&rect);
                g_is_clipped = true;
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void StartMouseHook()
{
    if (!g_mouse_hook)
    {
        g_mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    }
}

void StopMouseHook()
{
    if (g_mouse_hook)
    {
        UnhookWindowsHookEx(g_mouse_hook);
        g_mouse_hook = NULL;
    }
    ClipCursor(NULL);
    g_is_clipped = false;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if ((HWND)lParam == g_hwnd_button)
        {
            g_is_running = !g_is_running;
            if (g_is_running)
            {
                SetWindowText(g_hwnd_button, "Stop Lock");
                SetWindowText(g_hwnd_label, "Status: Running");
                StartMouseHook();
            }
            else
            {
                SetWindowText(g_hwnd_button, "Start Lock");
                SetWindowText(g_hwnd_label, "Status: Idle");
                StopMouseHook();
            }
        }
        break;
    case WM_DESTROY:
        StopMouseHook();
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    const char kClassName[] = "CursorLockWindow";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = kClassName;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, kClassName, "Cursor Lock for Brawlhalla",
                               WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
                               CW_USEDEFAULT, CW_USEDEFAULT, 350, 120,
                               NULL, NULL, hInstance, NULL);

    g_hwnd_label = CreateWindow("STATIC", "Status: Idle",
                             WS_CHILD | WS_VISIBLE,
                             20, 10, 300, 20,
                             hwnd, NULL, hInstance, NULL);

    g_hwnd_button = CreateWindow("BUTTON", "Start Lock",
                              WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                              100, 40, 120, 30,
                              hwnd, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}