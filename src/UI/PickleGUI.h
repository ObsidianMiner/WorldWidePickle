#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx10.h"
#include <d3d10_1.h>
#include <d3d10.h>
#include <tchar.h>
#include <optional>
#include <string>

class Font
{
public:
	ImFont* regular;
	ImFont* bold;
	ImFont* italic;
	ImFont* boldItalic;

	Font(std::string regularPath, std::optional<std::string> boldPath = std::nullopt, std::optional<std::string> italicPath = std::nullopt, std::optional<std::string> boldItalicPath = std::nullopt);
	Font() = default;
};
class PGUI
{
public:
	static ImVec4 clear_color;
	static ImGuiIO* io;
	static UINT resizeWidth, resizeHeight;
	static bool swapChainIncluded;

	static bool firstFrame;
	static bool minimized;

	static IDXGISwapChain* swapChain;
	static ID3D10Device* pd3dDevice;
	static ID3D10RenderTargetView* mainRenderTargetView;

	static Font defaultFont;

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
	static void DockWindowToTabBar(const char* windowName);
private:
	static void SetupDockingLayout(ImGuiID dockspace_id);
	static ImGuiID tabBarDockId;
	static WNDCLASSEXW wc;
	static HWND hwnd;
};