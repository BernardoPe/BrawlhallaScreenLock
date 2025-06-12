import com.sun.jna.win32.StdCallLibrary

interface User32 : StdCallLibrary {
    fun FindWindowA(lpClassName: String?, lpWindowName: String?): HWND?
    fun GetForegroundWindow(): HWND?
    fun GetWindowRect(hWnd: HWND?, rect: RECT): Boolean
    fun GetCursorPos(point: POINT): Boolean
    fun SetCursorPos(x: Int, y: Int): Boolean
}
