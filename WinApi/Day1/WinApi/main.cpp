// Client.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "framework.h"
#include "Client.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst; // ���� �ν��Ͻ��Դϴ�.
HWND      g_hWnd;// Main Window Handle                                 

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// SAL
int APIENTRY wWinMain(HINSTANCE hInstance // ���α׷��� ����Ǿ����� �޸� ���� �ּ�
    , HINSTANCE hPrevInstance
    , LPWSTR lpCmdLine
    , int nCmdShow)
{
    // ���� ��ų ������ ����
    MyRegisterClass(hInstance);

    // ������ �ϳ� ����
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg = {};
    //LPMSG pMsg = &msg;

    // �⺻ �޽��� �����Դϴ�:
    while (true)
    {
        if (false == GetMessage(&msg, nullptr, 0, 0))
        {
            break;
        }

        // �޼��� ó��
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg); // �޼����� �߻��� �������� ���ν��� �Լ��� ȣ���Ų��. (�޼��� ó���� ���ؼ�)
        }
    }

    if (WM_QUIT == msg.message)
    {
        int a = 0;
    }


    return (int)msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName = L"MyWindowKey";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    // ������ ����
    // ������ ==> Ŀ�� ������Ʈ
    // ������Ų �������� �ڵ�(ID) �� �޴´�.
    g_hWnd = CreateWindowW(L"MyWindowKey", L"MyGame", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);


    if (!g_hWnd)
    {
        return FALSE;
    }

    ShowWindow(g_hWnd, true);
    UpdateWindow(g_hWnd);

    return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // �귯��, �� ����
        HBRUSH redBrush = CreateSolidBrush(RGB(100, 0, 255));
        HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 255, 100));

        // dc �� Brush �� ������
        HBRUSH prevBrush = (HBRUSH)SelectObject(hdc, redBrush);
        HPEN prevPen = (HPEN)SelectObject(hdc, hPen);

        // �׸���
        Rectangle(hdc, 100, 100, 200, 200);
        //Ellipse(hdc, 100, 100, 200, 200);

        // ���� ��, �귯���� �ǵ���
        SelectObject(hdc, prevBrush);
        SelectObject(hdc, prevPen);

        // ����� ��, �귯�� ����
        DeleteObject(redBrush);
        DeleteObject(hPen);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
