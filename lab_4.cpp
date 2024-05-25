#include <windows.h>
#include <math.h>

// Константы для размеров окна
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Функция для рисования одуванчика
void DrawDandelion(HDC hdc, int x, int y, int size) {
    HPEN hPen;
    HBRUSH hBrush;
    int i;
    double angle;

    // Рисуем стебель
    hPen = CreatePen(PS_SOLID, 2, RGB(0, 128, 0));
    SelectObject(hdc, hPen);
    MoveToEx(hdc, x, y, NULL);
    LineTo(hdc, x, y - size);
    DeleteObject(hPen);

    // Рисуем центр одуванчика
    hBrush = CreateSolidBrush(RGB(255, 255, 0));
    SelectObject(hdc, hBrush);
    Ellipse(hdc, x - size / 8, y - size - size / 8, x + size / 8, y - size + size / 8);
    DeleteObject(hBrush);

    // Рисуем пушинки
    hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, hPen);
    for (i = 0; i < 360; i += 15) {
        angle = i * 3.14159 / 180;
        MoveToEx(hdc, x, y - size, NULL);
        LineTo(hdc, x + (int)(cos(angle) * size / 2), y - size + (int)(sin(angle) * size / 2));
    }
    DeleteObject(hPen);
}

// Функция для рисования луга
void DrawMeadow(HDC hdc, int width, int height) {
    HBRUSH hBrush;

    // Рисуем траву
    hBrush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(hdc, hBrush);
    Rectangle(hdc, 0, height / 2, width, height);
    DeleteObject(hBrush);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    HWND hwnd;
    MSG msg;

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DandelionMeadow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    hwnd = CreateWindow(wc.lpszClassName, L"Dandelion Meadow", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;

    switch (uMsg) {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        DrawMeadow(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
        // Рисуем несколько одуванчиков
        DrawDandelion(hdc, 200, 500, 100);
        DrawDandelion(hdc, 400, 450, 80);
        DrawDandelion(hdc, 600, 500, 120);
        EndPaint(hwnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}