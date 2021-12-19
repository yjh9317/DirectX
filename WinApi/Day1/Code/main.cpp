#include "framework.h"
#include "Client.h"

#define MAX_LOADSTRING 100


HINSTANCE hInst; // 현재 인스턴스입니다.
HWND      g_hWnd;// Main Window Handle                                 

//HINSTANCE와 HWND는 둘다 4바이트 정수형을 가진 구조체이지만 typedef를 선언해줌으로써 다른 자료형이 되어 호환이 되지않는다.




// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/*
CALLBACK은 함수호출규약중 하나인 _stdcall를 의미
  많이 쓰이는 2개
_stdcall은 함수호출받은 함수에서 정리를 한다.
_cdecl은 호출한쪽에서 정리를한다.
*/




/* SAL(주석언어 소스코드)는 부연설명해주는 코드로써 대표적으로 _In_,_Out_,_InOut_등이 있다.
   _In_가 함수의 매개변수에 앞에 있으면 함수에 넣어주는 인자를 의미한다.
   _Out_은 함수의 매개변수 앞에 붙어있으면 함수의 반환값에 영향을 주는 변수를 의미한다.
   _InOut_은 _In_과_Out_ 둘다 적용되는 의미다.
*/




int APIENTRY wWinMain(HINSTANCE hInstance // 프로그램이 실행되었을때 메모리 시작 주소
    , HINSTANCE hPrevInstance // 이전 메모리 주소
    , LPWSTR lpCmdLine // 윈도우 함수를 명령창으로 실행할 때 명령어 문자를 받는 인자
    , int nCmdShow)
{
    // 생성 시킬 윈도우 설정
    MyRegisterClass(hInstance);

    // 윈도우 하나 생성
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT)); //조합키를 string table

    MSG msg = {};
    //MSG를 구현한 클래스로 직접가보면 typedef로 MSG, *PMSG, *NPMSG , *LPMSG로 선언이 돼있다.
    //여기서 *LPMSG나 *PMSG같이 되어있는 변수들은 밑에처럼 포인터느낌으로 사용할 수 있다
    //LPMSG pMsg = &msg;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (false == GetMessage(&msg, nullptr, 0, 0))
        {
            break;
        }

        // 메세지 처리
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg); // 메세지가 발생한 윈도우의 프로시저 함수를 호출시킨다. (메세지 처리를 위해서)
        }
    }

    if (WM_QUIT == msg.message)
    {
        int a = 0; //WM_QUIT 중단점 확인용
    }


    return (int)msg.wParam;
}




//  함수: MyRegisterClass()
//  용도: 창 클래스를 등록합니다.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;                                                           //WNDCLASSEXW : 윈도우의 설정들을 모아놓은 구조체

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;                                              // 이벤트가 발생할 때 처리해줄 함수의 주소를 받는 변수
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;                                                // MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName = L"MyWindowKey";                                       // 구조체 설정의 키
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


//   함수: InitInstance(HINSTANCE, int)
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//   주석:
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    // 윈도우 생성
    // 윈도우 ==> 커널 오브젝트
    // 생성시킨 윈도우의 핸들(ID) 를 받는다.
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


//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//  용도: 주 창의 메시지를 처리합니다.
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
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

        // 브러쉬, 펜 생성
        HBRUSH redBrush = CreateSolidBrush(RGB(100, 0, 255));
        HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 255, 100));

        // dc 가 Brush 를 선택함
        HBRUSH prevBrush = (HBRUSH)SelectObject(hdc, redBrush);
        HPEN prevPen = (HPEN)SelectObject(hdc, hPen);

        // 그리기
        Rectangle(hdc, 100, 100, 200, 200);
        //Ellipse(hdc, 100, 100, 200, 200);

        // 이전 펜, 브러쉬도 되돌림
        SelectObject(hdc, prevBrush);
        SelectObject(hdc, prevPen);

        // 사용한 펜, 브러쉬 삭제
        DeleteObject(redBrush);
        DeleteObject(hPen);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:        //message에 WM_DESTORY가 들어와서 여기에 걸리고나면 PostQuitMessage(0)이 실행되면서 메세지큐에 WM_Quit를 넣으면서 false를 반환하고 반복문이 종료됨.
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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