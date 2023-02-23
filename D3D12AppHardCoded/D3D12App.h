
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
	
	HINSTANCE m_hInstance;
	int m_nCmdShow;
	HWND m_hWnd;

	int m_WindowWidth{ 800 };
	int m_WindowHeight{ 600 };
};

#endif
