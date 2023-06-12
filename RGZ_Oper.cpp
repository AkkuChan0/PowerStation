/*
ГЭС имеет 4 гидрогенератора и снабжает электроэнергией 3 объекта. Мощность, потребляемая каждым объектом, 
меняется в течение суток относительно некоторого среднего значения по синусоидальному закону. Мощности гидрогенераторов фиксированы:
Ргг1 = 100 МВт
Ргг2 = 100 МВт
Ргг3 = 120 МВт
Ргг4 = 120 МВт
Мощность, потребляемая объектами (МВт):
Р1= 100 + 20*Sin(pi/2*(h/6 - 1))
Р2= 180 + 50*Sin(pi/2*(h/6 - 1))
Р3=90+ 20*Sin(pi/2*(h/6 - 1))
Где h – текущий час в течение суток (0 - 23).
Написать программу, моделирующую работу энергосистемы. Временной масштаб моделирования 1 : 3600 (1 Сек соответствует 1 часу). 
Моделирование потребления электроэнергии каждым объектом выполняется в отдельном потоке. 
Первичный поток процесса получает данные от каждого объекта и при необходимости отключает (или включает) гидрогенератор 4, 
в зависимости от того, хватает ли суммарной мощности первых трех генераторов для энергоснабжения объектов. 
На главной форме визуально должны отображаться текущие мощности потребления всех объектов, а также состояние всех генераторов (включен -отключен).
*/

#include <string>
#include "RGZ_Oper.h"
#include "Director.h"

Director GES;
bool isWork = false;

// функция обновления отображения данных
void UpdatePowerAndStatus(HWND hWnd) 
{
    double sumP = GES.getPower(0) + GES.getPower(1) + GES.getPower(2);

    SetWindowTextA(GetDlgItem(hWnd, IDC_POWER1), (" " + std::to_string(GES.getPower(0))).c_str());
    SetWindowTextA(GetDlgItem(hWnd, IDC_POWER2), (" " + std::to_string(GES.getPower(1))).c_str());
    SetWindowTextA(GetDlgItem(hWnd, IDC_POWER3), (" " + std::to_string(GES.getPower(2))).c_str());

    SetWindowTextA(GetDlgItem(hWnd, IDC_HOUR), (" " + std::to_string(int(GES.getTime())) + ":00").c_str());
    SetWindowTextA(GetDlgItem(hWnd, IDC_POWER), (" " + std::to_string(sumP)).c_str());
    if (sumP <= 320)
    {
        SetWindowText(GetDlgItem(hWnd, IDC_GEN4), L"   OFF");
        SetWindowTextA(GetDlgItem(hWnd, IDC_GEN), (" " + std::to_string(320)).c_str());
    }
    else
    {
        SetWindowText(GetDlgItem(hWnd, IDC_GEN4), L"   ON");
        SetWindowTextA(GetDlgItem(hWnd, IDC_GEN), (" " + std::to_string(440)).c_str());
    }
}

//основной цикл сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg)
    {
    case WM_CREATE:
        //создание элементов управления на форме
        CreateWindowEx(0, L"STATIC", L"Object 1 Power:", WS_CHILD | WS_VISIBLE, 10, 10, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"Object 2 Power:", WS_CHILD | WS_VISIBLE, 10, 40, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"Object 3 Power:", WS_CHILD | WS_VISIBLE, 10, 70, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"Generator 1:", WS_CHILD | WS_VISIBLE, 10, 110, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"Generator 2:", WS_CHILD | WS_VISIBLE, 10, 140, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"Generator 3:", WS_CHILD | WS_VISIBLE, 10, 170, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"Generator 4:", WS_CHILD | WS_VISIBLE, 10, 200, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L" ", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 10, 90, 20, hWnd, (HMENU)IDC_POWER1, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L" ", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 40, 90, 20, hWnd, (HMENU)IDC_POWER2, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L" ", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 70, 90, 20, hWnd, (HMENU)IDC_POWER3, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"   ON", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 110, 50, 20, hWnd, (HMENU)IDC_GEN1, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"   ON", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 140, 50, 20, hWnd, (HMENU)IDC_GEN2, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"   ON", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 170, 50, 20, hWnd, (HMENU)IDC_GEN3, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"   OFF", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 200, 50, 20, hWnd, (HMENU)IDC_GEN4, nullptr, nullptr);

        CreateWindowEx(0, L"STATIC", L"  Hour:", WS_CHILD | WS_VISIBLE, 250, 10, 70, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 330, 10, 100, 20, hWnd, (HMENU)IDC_HOUR, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"  Power:", WS_CHILD | WS_VISIBLE, 250, 40, 70, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 330, 40, 100, 20, hWnd, (HMENU)IDC_POWER, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"  Gen:", WS_CHILD | WS_VISIBLE, 250, 70, 70, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 330, 70, 100, 20, hWnd, (HMENU)IDC_GEN, nullptr, nullptr);

        CreateWindowEx(0, L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE | WS_BORDER, 250, 200, 70, 20, hWnd, (HMENU)IDC_START, nullptr, nullptr);

        SetTimer(hWnd, 1, 500, NULL);

        break;


    case WM_TIMER:
        if (wParam == ID_TIMER1)
            UpdatePowerAndStatus(hWnd);
        break;

    case WM_CLOSE:
        //остановка потоков и освобождение ресурсов
        GES.stopWork();
        DestroyWindow(hWnd);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_START:
            if (isWork)
            {
                GES.stopWork();
                SetWindowText(GetDlgItem(hWnd, IDC_START), L"Start");
                isWork = false;
            }
            else
            {
                GES.startWork();
                SetWindowText(GetDlgItem(hWnd, IDC_START), L"Stop");
                isWork = true;
            }
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) 
{
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"MainWindowClass";
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassEx(&wcex);

    hWnd = CreateWindowEx(0, L"MainWindowClass", L"GES Simulation", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 270, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) 
    {
        return false;
    }

    //отображение и обновление главного окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    //основной цикл сообщений
    while (GetMessage(&msg, nullptr, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
