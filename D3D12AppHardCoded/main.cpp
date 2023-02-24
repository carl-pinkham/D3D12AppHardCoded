// d3d12.lib, dxgi.lib, and d3dcompiler.lib set in Project settings

// TO DO:

// update to winrt::com_ptr
// check HRESULTs
// validate m_hWnd
// PeekMessage in message loop
// build geometries, materials, and textures
// build shaders and input layout
// create vertex, index, texture, and constant buffers
// create RootSignature and PipelineState
// mouse and keyboard messages
// update OnUpdate and OnRender
// -----------------------------------------------------------------
// debug
// exceptions

#include "main.h"
#include "D3D12App.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!DirectX::XMVerifyCPUSupport())
    {
#ifdef _DEBUG
        OutputDebugStringW(L"ERROR: This hardware does not support the required instruction set.\n");
#endif
        return 1;
    }

    D3D12App app(hInstance, nCmdShow);
    app.InitializeWindow();
    app.InitializeD3D12();
    return app.RunMessageLoop();
}
