
#include "D3D12App.h"

D3D12App::D3D12App(HINSTANCE hInstance, int nCmdShow) : m_hInstance{ hInstance }, m_nCmdShow{ nCmdShow }, m_hWnd{ NULL }
{
    m_DXGIFactory = nullptr;
    m_D3D12Device = nullptr;
    m_D3D12CommandQueue = nullptr;
    m_D3D12CommandAllocator = nullptr;
    m_D3D12GraphicsCommandList = nullptr;
    m_DXGISwapChain = nullptr;
    m_SwapChainBuffer[0] = nullptr;
    m_SwapChainBuffer[1] = nullptr;
    m_DepthStencilBuffer = nullptr;
    m_RTVDescriptorHeap = nullptr;
    m_DSVDescriptorHeap = nullptr;
    m_D3D12Fence = nullptr;
    m_D3D12RootSignature = nullptr;
    m_D3D12PipelineState = nullptr;
}

D3D12App::~D3D12App()
{
    // m_D3D12PipelineState->Release();
    // m_D3D12RootSignature->Release();
     m_D3D12Fence->Release();
     m_DSVDescriptorHeap->Release();
     m_RTVDescriptorHeap->Release();
     m_DepthStencilBuffer->Release();
     m_SwapChainBuffer[1]->Release();
     m_SwapChainBuffer[0]->Release();
     m_DXGISwapChain->Release();
     m_D3D12GraphicsCommandList->Release();
     m_D3D12CommandAllocator->Release();
     m_D3D12CommandQueue->Release();
     m_D3D12Device->Release();
     m_DXGIFactory->Release();
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
    DWORD DXGIFactoryFlags = 0;

    CreateDXGIFactory2(DXGIFactoryFlags, IID_PPV_ARGS(&m_DXGIFactory));

    D3D12CreateDevice(nullptr, m_FeatureLevel, IID_PPV_ARGS(&m_D3D12Device));

    m_D3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_D3D12Fence));

    m_RTVDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_DSVDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

    D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};
    CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    m_D3D12Device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&m_D3D12CommandQueue));

    m_D3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_D3D12CommandAllocator));

    m_D3D12Device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&m_D3D12GraphicsCommandList));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = m_WindowWidth;
    swapChainDesc.Height = m_WindowHeight;
    swapChainDesc.Format = m_BackBufferFormat;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = m_NumberOfSwapChainBuffers;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
    fsSwapChainDesc.Windowed = TRUE;

    IDXGISwapChain1* swapChain;
    m_DXGIFactory->CreateSwapChainForHwnd(m_D3D12CommandQueue, m_hWnd, &swapChainDesc, &fsSwapChainDesc, nullptr, &swapChain);
    m_DXGISwapChain = reinterpret_cast<IDXGISwapChain4*>(swapChain);

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    rtvHeapDesc.NumDescriptors = m_NumberOfSwapChainBuffers;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;
    m_D3D12Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RTVDescriptorHeap));

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDesc.NodeMask = 0;
    m_D3D12Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DSVDescriptorHeap));
    
    FlushCommandQueue();

    m_D3D12GraphicsCommandList->Reset(m_D3D12CommandAllocator, nullptr);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < m_NumberOfSwapChainBuffers; i++)
    {
        m_DXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i]));
        m_D3D12Device->CreateRenderTargetView(m_SwapChainBuffer[i], nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, m_RTVDescriptorSize);
    }

    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = m_WindowWidth;
    depthStencilDesc.Height = m_WindowHeight;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear;
    optClear.Format = m_DepthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;
    CD3DX12_HEAP_PROPERTIES a_HeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    m_D3D12Device->CreateCommittedResource(
        &a_HeapProp,
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(&m_DepthStencilBuffer));

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = m_DepthStencilFormat;
    dsvDesc.Texture2D.MipSlice = 0;
    m_D3D12Device->CreateDepthStencilView(m_DepthStencilBuffer, &dsvDesc, m_DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    CD3DX12_RESOURCE_BARRIER a_Resourcebarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    m_D3D12GraphicsCommandList->ResourceBarrier(1, &a_Resourcebarrier);

    m_D3D12GraphicsCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { m_D3D12GraphicsCommandList };
    m_D3D12CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until resize is complete.
    FlushCommandQueue();

    // Update the viewport transform to cover the client area.
    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(m_WindowWidth);
    m_ScreenViewport.Height = static_cast<float>(m_WindowHeight);
    m_ScreenViewport.MinDepth = 0.0f;
    m_ScreenViewport.MaxDepth = 1.0f;

    m_ScissorRect = { 0, 0, m_WindowWidth, m_WindowHeight };

    m_D3D12GraphicsCommandList->Reset(m_D3D12CommandAllocator, nullptr);


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

void D3D12App::FlushCommandQueue()
{
    m_CurrentFence++;
    m_D3D12CommandQueue->Signal(m_D3D12Fence, m_CurrentFence);
    if (m_D3D12Fence->GetCompletedValue() < m_CurrentFence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
        m_D3D12Fence->SetEventOnCompletion(m_CurrentFence, eventHandle);
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
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
