#pragma once

#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_win32.h"
#include "../imgui/backends/imgui_impl_dx10.h"
#include <d3d10_1.h>
#include <d3d10.h>
#include <tchar.h>

class PGUI
{
public:
	static ImVec4 clear_color;
	static ImGuiIO* io;
	static WNDCLASSEXW wc;
	static HWND hwnd;
	static UINT resizeWidth, resizeHeight;
	static bool swapChainIncluded;
	static bool firstFrame;
	static IDXGISwapChain* swapChain;
	static ID3D10Device* pd3dDevice;
	static ID3D10RenderTargetView* mainRenderTargetView;
	PGUI();
	bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CleanupRenderTarget();
	static void CreateRenderTarget();
	static void Cleanup();
	/// <summary>
	/// Handles window-related operations or events.
	/// </summary>
	/// <returns>Returns true if the window is no more</returns>
	static bool HandleWindow();
	/// <summary>
	/// Call every frame after creating all imgui windows
	/// </summary>
	static void Render();
	/// <summary>
	/// Call at the begining of each frame
	/// </summary>
	static void StartFrame();
};