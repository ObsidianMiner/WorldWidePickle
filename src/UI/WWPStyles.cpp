/**
* @file WWPStyles.cpp
* @brief Holds browser ImGui styles.
*/

#include "WWPStyles.h"

void ApplyStyle(int theme)
{
	// "Purple","Cherry","Light","DarkBlue","Everfrost","ClasicSteam"
	switch (theme)
	{
	case 5:
		SetupClasicSteamStyle();
		break;
	case 4:
		SetupEverfrostStyle();
		break;
	case 3:
		SetupDarkBlueStyle();
		break;
	case 2:
		SetupLightStyle();
		break;
	case 1:
		SetupCherryStyle();
		break;
	default:
		SetPurpleStyle();
		break;
	}
}

ImGuiStyle SetupEverfrostStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6f;
	style.WindowPadding = ImVec2(6.0f, 3.0f);
	style.WindowRounding = 6.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(5.0f, 1.0f);
	style.FrameRounding = 3.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 13.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 1.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.8745098f, 0.87058824f, 0.8392157f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.58431375f, 0.57254905f, 0.52156866f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.59607846f, 0.5921569f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.59607846f, 0.5921569f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 0.972549f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.8392157f, 0.7490196f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.8392157f, 0.7490196f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.8392157f, 0.7490196f, 0.4f, 0.60944206f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.8392157f, 0.7490196f, 0.4f, 0.43137255f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.8392157f, 0.7490196f, 0.4f, 0.9019608f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);

	return style;
}

ImGuiStyle SetupClasicSteamStyle()
{
	// Classic Steam style by metasprite from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.28627452f, 0.3372549f, 0.25882354f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.28627452f, 0.3372549f, 0.25882354f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.23921569f, 0.26666668f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.13725491f, 0.15686275f, 0.10980392f, 0.52f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.23921569f, 0.26666668f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26666668f, 0.29803923f, 0.22745098f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.29803923f, 0.3372549f, 0.25882354f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.23921569f, 0.26666668f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.28627452f, 0.3372549f, 0.25882354f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.23921569f, 0.26666668f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2784314f, 0.31764707f, 0.23921569f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24705882f, 0.29803923f, 0.21960784f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.22745098f, 0.26666668f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 0.5f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.28627452f, 0.3372549f, 0.25882354f, 0.4f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 0.5f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 0.6f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 0.5f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.13725491f, 0.15686275f, 0.10980392f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1882353f, 0.22745098f, 0.1764706f, 0.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 0.78f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.23921569f, 0.26666668f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.60784316f, 0.60784316f, 0.60784316f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.7764706f, 0.2784314f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.7294118f, 0.6666667f, 0.23921569f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
	return style;
}

ImGuiStyle SetupCherryStyle()
{
	// Cherry style from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6f;
	style.WindowPadding = ImVec2(6.0f, 3.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(5.0f, 1.0f);
	style.FrameRounding = 3.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 13.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 1.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.85882354f, 0.92941177f, 0.8862745f, 0.88f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85882354f, 0.92941177f, 0.8862745f, 0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12941177f, 0.13725491f, 0.16862746f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2f, 0.21960784f, 0.26666668f, 0.9f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.5372549f, 0.47843137f, 0.25490198f, 0.162f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.21960784f, 0.26666668f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.23137255f, 0.2f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.5019608f, 0.07450981f, 0.25490198f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.21960784f, 0.26666668f, 0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2f, 0.21960784f, 0.26666668f, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2f, 0.21960784f, 0.26666668f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.08627451f, 0.14901961f, 0.15686275f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.46666667f, 0.76862746f, 0.827451f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.5019608f, 0.07450981f, 0.25490198f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.42745098f, 0.42745098f, 0.49803922f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803922f, 0.4f, 0.7490196f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803922f, 0.4f, 0.7490196f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.46666667f, 0.76862746f, 0.827451f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1764706f, 0.34901962f, 0.5764706f, 0.862f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.19607843f, 0.40784314f, 0.6784314f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667f, 0.101960786f, 0.14509805f, 0.9724f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13333334f, 0.25882354f, 0.42352942f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85882354f, 0.92941177f, 0.8862745f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85882354f, 0.92941177f, 0.8862745f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.45490196f, 0.19607843f, 0.29803923f, 0.43f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
	return style;
}

ImGuiStyle SetupDarkBlueStyle()
{
	// Dark style from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882353f, 0.05882353f, 0.05882353f, 0.94f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.078431375f, 0.078431375f, 0.078431375f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.42745098f, 0.42745098f, 0.49803922f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15686275f, 0.28627452f, 0.47843137f, 0.54f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.4f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.039215688f, 0.039215688f, 0.039215688f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15686275f, 0.28627452f, 0.47843137f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.13725491f, 0.13725491f, 0.13725491f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.019607844f, 0.019607844f, 0.019607844f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30980393f, 0.30980393f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40784314f, 0.40784314f, 0.40784314f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50980395f, 0.50980395f, 0.50980395f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.4f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05882353f, 0.5294118f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.42745098f, 0.42745098f, 0.49803922f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803922f, 0.4f, 0.7490196f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803922f, 0.4f, 0.7490196f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.2f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1764706f, 0.34901962f, 0.5764706f, 0.862f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.19607843f, 0.40784314f, 0.6784314f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667f, 0.101960786f, 0.14509805f, 0.9724f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13333334f, 0.25882354f, 0.42352942f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.60784316f, 0.60784316f, 0.60784316f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.42745098f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392f, 0.69803923f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
	return style;
}
ImGuiStyle SetupLightStyle()
{
	// Light style from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.9372549f, 0.9372549f, 0.9372549f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.98f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.3f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.4f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.95686275f, 0.95686275f, 0.95686275f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.81960785f, 0.81960785f, 0.81960785f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 1.0f, 1.0f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.85882354f, 0.85882354f, 0.85882354f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.9764706f, 0.9764706f, 0.9764706f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.6862745f, 0.6862745f, 0.6862745f, 0.8f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4862745f, 0.4862745f, 0.4862745f, 0.8f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4862745f, 0.4862745f, 0.4862745f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.4f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05882353f, 0.5294118f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.3882353f, 0.3882353f, 0.3882353f, 0.62f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.13725491f, 0.4392157f, 0.8f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.13725491f, 0.4392157f, 0.8f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.34901962f, 0.34901962f, 0.34901962f, 0.17f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.7607843f, 0.79607844f, 0.8352941f, 0.931f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5921569f, 0.7254902f, 0.88235295f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.91764706f, 0.9254902f, 0.93333334f, 0.9862f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7411765f, 0.81960785f, 0.9137255f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.3882353f, 0.3882353f, 0.3882353f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.42745098f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392f, 0.69803923f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.44705883f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.7764706f, 0.8666667f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.5686275f, 0.5686275f, 0.6392157f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.6784314f, 0.6784314f, 0.7372549f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.29803923f, 0.29803923f, 0.29803923f, 0.09f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.95f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.69803923f, 0.69803923f, 0.69803923f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.35f);
	return style;
}
ImGuiStyle SetPurpleStyle()
{
	// Purple Comfy style by RegularLunar from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.1f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 10.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(30.0f, 30.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 5.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 10.0f;
	style.PopupBorderSize = 0.0f;
	style.FramePadding = ImVec2(5.0f, 3.5f);
	style.FrameRounding = 5.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(5.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 5.0f;
	style.ColumnsMinSpacing = 5.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 15.0f;
	style.GrabRounding = 5.0f;
	style.TabRounding = 5.0f;
	style.TabBorderSize = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 0.360515f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15686275f, 0.15686275f, 0.15686275f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38039216f, 0.42352942f, 0.57254905f, 0.54901963f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.25882354f, 0.25882354f, 0.25882354f, 0.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15686275f, 0.15686275f, 0.15686275f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.15686275f, 0.15686275f, 0.15686275f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.23529412f, 0.23529412f, 0.23529412f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.29411766f, 0.29411766f, 0.29411766f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.4509804f, 1.0f, 0.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13333334f, 0.25882354f, 0.42352942f, 0.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.29411766f, 0.29411766f, 0.29411766f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.7372549f, 0.69411767f, 0.8862745f, 0.54901963f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.2901961f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03433478f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.5019608f, 0.3019608f, 1.0f, 0.54901963f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
	return style;
}