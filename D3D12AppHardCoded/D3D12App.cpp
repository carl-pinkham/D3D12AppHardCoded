
#include "D3D12App.h"

D3D12App::D3D12App(HINSTANCE hInstance, int nCmdShow) : m_hInstance{ hInstance }, m_nCmdShow{ nCmdShow }, m_hWnd{ NULL }
{

}

D3D12App::~D3D12App()
{

}

void D3D12App::InitializeWindow()
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = D3D12App::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"D3D12App";
    wcex.hIconSm = NULL;
    RegisterClassExW(&wcex);

    RECT a_rect = { 0, 0, static_cast<LONG>(m_WindowWidth), static_cast<LONG>(m_WindowHeight) };
    AdjustWindowRect(&a_rect, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWnd = CreateWindowW(L"D3D12App", L"D3D12 App", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, a_rect.right - a_rect.left, a_rect.bottom - a_rect.top, nullptr, nullptr, m_hInstance, nullptr);
    ShowWindow(m_hWnd, m_nCmdShow);

    SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

void D3D12App::InitializeD3D12()
{

}

int D3D12App::RunMessageLoop()
{
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void D3D12App::OnUpdate()
{

}

void D3D12App::OnRender()
{
    PAINTSTRUCT a_ps;
    HDC hdc = BeginPaint(m_hWnd, &a_ps);
    RECT a_rect{ 10, 10, 790, 50 };
    DrawTextW(hdc, L"D3D12 App Under Construction...", 32, &a_rect, DT_CENTER);
    EndPaint(m_hWnd, &a_ps);
}

LRESULT CALLBACK D3D12App::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    D3D12App* p_d3d12app = reinterpret_cast<D3D12App*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    switch (message)
    {
    case WM_PAINT:
    {
        p_d3d12app->OnUpdate();
        p_d3d12app->OnRender();
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
