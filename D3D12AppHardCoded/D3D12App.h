
#ifndef D3D12APP_H_
#define D3D12APP_H_

#include "main.h"

class D3D12App
{
public:
	D3D12App(HINSTANCE hInstance, int nCmdShow);
	~D3D12App();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void InitializeWindow();
	void InitializeD3D12();
	int RunMessageLoop();

private:
	void OnUpdate();
	void OnRender();

	void FlushCommandQueue();
	
	HINSTANCE m_hInstance;
	int m_nCmdShow;
	HWND m_hWnd;

	int m_WindowWidth{ 800 };
	int m_WindowHeight{ 600 };

	IDXGIFactory7* m_DXGIFactory;

	ID3D12Device9* m_D3D12Device;

	ID3D12CommandQueue* m_D3D12CommandQueue;
	ID3D12CommandAllocator* m_D3D12CommandAllocator;
	ID3D12GraphicsCommandList5* m_D3D12GraphicsCommandList;

	IDXGISwapChain4* m_DXGISwapChain;
	static const UINT m_NumberOfSwapChainBuffers{ 2 };
	UINT m_BackBufferIndex;
	ID3D12Resource* m_SwapChainBuffer[m_NumberOfSwapChainBuffers];
	ID3D12Resource* m_DepthStencilBuffer;

	UINT m_RTVDescriptorSize;
	UINT m_DSVDescriptorSize;
	ID3D12DescriptorHeap* m_RTVDescriptorHeap;
	ID3D12DescriptorHeap* m_DSVDescriptorHeap;

	ID3D12Fence1* m_D3D12Fence;
	UINT64 m_CurrentFence{ 0 };

	ID3D12RootSignature* m_D3D12RootSignature;
	ID3D12PipelineState* m_D3D12PipelineState;

	D3D12_VIEWPORT m_ScreenViewport;
	D3D12_RECT m_ScissorRect;

	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	D3D_FEATURE_LEVEL m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

};

#endif // D3D12APP_H_
