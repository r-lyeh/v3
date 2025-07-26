#ifndef CIMGUI_H
#define CIMGUI_H

#elif CODE
#pragma once
#include "glob.cc"

extern "C"
void igCherryTheme() {
    ImGui::StyleColorsDark();    // Reset styles

    // cherry colors, 3 intensities
    #define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
    #define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
    #define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
    // backgrounds (@todo: complete with BG_MED, BG_LOW)
    #define BG(v)   ImVec4(0.200f, 0.220f, 0.270f, v)
    // text
    #define TXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

    auto &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]                  = TXT(0.78f);
    style.Colors[ImGuiCol_TextDisabled]          = TXT(0.28f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_ChildBg]               = BG( 0.58f);
    style.Colors[ImGuiCol_PopupBg]               = BG( 0.9f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = BG( 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = MED( 0.78f);
    style.Colors[ImGuiCol_FrameBgActive]         = HI( 1.00f);
    style.Colors[ImGuiCol_TitleBg]               = LOW( 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = HI( 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = BG( 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]             = BG( 0.47f);
    style.Colors[ImGuiCol_ScrollbarBg]           = BG( 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = MED( 0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = HI( 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_ButtonHovered]         = MED( 0.86f);
    style.Colors[ImGuiCol_ButtonActive]          = MED( 1.00f);
    style.Colors[ImGuiCol_Header]                = MED( 0.76f);
    style.Colors[ImGuiCol_HeaderHovered]         = MED( 0.86f);
    style.Colors[ImGuiCol_HeaderActive]          = HI( 1.00f);
//  style.Colors[ImGuiCol_Column]                = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
//  style.Colors[ImGuiCol_ColumnHovered]         = MED( 0.78f);
//  style.Colors[ImGuiCol_ColumnActive]          = MED( 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = MED( 0.78f);
    style.Colors[ImGuiCol_ResizeGripActive]      = MED( 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = TXT(0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = MED( 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = TXT(0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = MED( 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = MED( 0.43f);
    // [...]
    style.Colors[ImGuiCol_ModalWindowDimBg]      = BG( 0.73f);
    style.Colors[ImGuiCol_Header]                 = style.Colors[ImGuiCol_Button];
    style.Colors[ImGuiCol_ButtonActive]           = style.Colors[ImGuiCol_TitleBgActive];
    style.Colors[ImGuiCol_ResizeGripActive]       = style.Colors[ImGuiCol_TitleBgActive];

    style.Colors[ImGuiCol_Tab]                    = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    style.Colors[ImGuiCol_TabHovered]             = ImVec4(0.20f, 0.38f, 0.62f, 0.96f);
    style.Colors[ImGuiCol_TabSelected]            = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    style.Colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
    style.Colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.00f, 0.33f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
#if 0
    style.colors[ImGuiCol_TabHovered]             = ImVec4(0.39f, 0.00f, 0.17f, 1.00f);
    style.colors[ImGuiCol_Tab]                    = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    style.colors[ImGuiCol_TabSelected]            = ImVec4(0.25f, 0.03f, 0.12f, 1.00f);
    style.colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.39f, 0.00f, 0.17f, 0.51f);
#endif



    style.FrameRounding            = 3.0f;
    style.GrabRounding             = 2.0f;
    style.GrabMinSize              = 20.0f;
    style.ScrollbarSize            = 12.0f;
    style.ScrollbarRounding        = 16.0f;

    style.ItemSpacing.x            = 4;
    style.FramePadding.y = 0;
    style.ItemSpacing.y = 2;
#if 0
    style.WindowPadding            = ImVec2(6, 4);
    style.WindowRounding           = 0.0f;
    style.FramePadding             = ImVec2(5, 2);
    style.ItemSpacing              = ImVec2(7, 1);
    style.ItemInnerSpacing         = ImVec2(1, 1);
    style.TouchExtraPadding        = ImVec2(0, 0);
    style.IndentSpacing            = 6.0f;
#endif

    style.WindowTitleAlign.x = 0.50f;
    style.SelectableTextAlign.x = 0.50f;
    style.SeparatorTextAlign.x = 0.04f;
    style.SeparatorTextBorderSize = 1;
    style.SeparatorTextPadding = ImVec2(0,0);

    //style.Colors[ImGuiCol_Header].w = 0/255.f; // collapsable headers
    style.Colors[ImGuiCol_TableBorderLight].w = 80/255.f; // column resizing grips

    style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
    style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 1.0f;

    #undef HI
    #undef MED
    #undef LOW
    #undef BG
    #undef TXT
}

extern "C"
void igTextWithHoverColor(ImU32 col, ImVec2 indents_offon, const char* text_begin) {
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    const char *text_end = text_begin + strlen(text_begin);

    // Layout
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const ImVec2 text_size = CalcTextSize(text_begin, text_end);
    ImRect bb(text_pos.x, text_pos.y, text_pos.x + text_size.x, text_pos.y + text_size.y);
    ItemSize(text_size, 0.0f);
    if (!ItemAdd(bb, 0))
        return;

    // Render
    bool hovered = IsItemHovered();
    if (hovered) PushStyleColor(ImGuiCol_Text, col);
    RenderText(bb.Min + ImVec2(hovered ? indents_offon.y : indents_offon.x,0), text_begin, text_end, false);
    if (hovered) PopStyleColor();
}

extern "C"
int igCurrentWindowStackSize(void) {
    using namespace ImGui;
    return GImGui->CurrentWindowStack.Size;
}

extern "C"
int igIsAnyWindowHovered(void) {
    return igIsWindowHovered(ImGuiHoveredFlags_AnyWindow);
}

extern "C"
float igGetWindowPosX(void) { ImVec2 v; return igGetWindowPos(&v), v.x; }

extern "C"
float igGetWindowPosY(void) { ImVec2 v; return igGetWindowPos(&v), v.y; }

extern "C"
int igRightAlign(const char* str_id) {
    if(igBeginTable(str_id, 2, ImGuiTableFlags_SizingFixedFit, ImVec2(-1,0), 0)) {
        igTableSetupColumn("a", ImGuiTableColumnFlags_WidthStretch, 0,0);

        igTableNextColumn();
        igTableNextColumn();
        return true;
    }
    return false;
}
extern "C"
int igRightAlignEnd() {
    return igEndTable(), false;
}

/*
extern "C" int igMouseLastCursor() {
    ImGui_ImplSDL3_Data* bd = ImGui_ImplSDL3_GetBackendData();
    for( int i = ImGuiMouseCursor_Arrow; i < ImGuiMouseCursor_COUNT; ++i ) 
        if( bd->MouseLastCursor == bd->MouseCursors[i] ) return i;
    return ImGuiMouseCursor_Arrow;
}
*/

extern "C"
int igThemeV3(int hue07, int alt07, int nav07, int lit01 = 0, int compact01 = 0, int border01 = 0, int shape0123 = 1) {
    bool rounded = shape0123 == 2;

    // V3 style from ImThemes
    ImGuiStyle& style = ImGui::GetStyle();
    
    const float _8 = compact01 ? 4 : 8;
    const float _4 = compact01 ? 2 : 4;
    const float _2 = compact01 ? 0.5 : 1;

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.3f;

    style.WindowPadding = ImVec2(4, _8);
    style.FramePadding = ImVec2(4, _4);
    style.ItemSpacing = ImVec2(_8, _2 + _2);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.IndentSpacing = 16;
    style.ScrollbarSize = compact01 ? 12 : 18;
    style.GrabMinSize = compact01 ? 16 : 20;

    style.WindowBorderSize = border01;
    style.ChildBorderSize = border01;
    style.PopupBorderSize = border01;
    style.FrameBorderSize = 0;

    style.WindowRounding = 4;
    style.ChildRounding = 6;
    style.FrameRounding = shape0123 == 0 ? 0 : shape0123 == 1 ? 4 : 12;
    style.PopupRounding = 4;
    style.ScrollbarRounding = rounded * 8 + 4;
    style.GrabRounding = style.FrameRounding;

    style.TabBorderSize = 0;
    style.TabBarBorderSize = 2;
    style.TabBarOverlineSize = 2;
    style.TabCloseButtonMinWidthSelected = -1; // -1:always visible, 0:visible when hovered, >0:visible when hovered if minimum width
    style.TabCloseButtonMinWidthUnselected = -1;
    style.TabRounding = rounded;

    style.CellPadding = ImVec2(8.0f, 4.0f);

    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Right;

    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.5f, 0.5f);
    style.SeparatorTextAlign.x = 1.00f;
    style.SeparatorTextBorderSize = 1;
    style.SeparatorTextPadding = ImVec2(0,0);

    style.WindowMinSize = ImVec2(32.0f, 16.0f);
    style.ColumnsMinSpacing = 6.0f;

    // diamond sliders
    style.CircleTessellationMaxError = shape0123 == 3 ? 4.00f : 0.30f; 

    auto lit = [&](ImVec4 hi) {
        float h,s,v; ImGui::ColorConvertRGBtoHSV(hi.x,hi.y,hi.z, h,s,v);
        ImVec4 lit = ImColor::HSV(h,s*0.80,v*1.00, hi.w).Value;
        return lit;
    };
    auto dim = [&](ImVec4 hi) {
        float h,s,v; ImGui::ColorConvertRGBtoHSV(hi.x,hi.y,hi.z, h,s,v);
        ImVec4 dim = ImColor::HSV(h,s,lit01 ? v*0.65:v*0.65, hi.w).Value;
        if( hi.z > hi.x && hi.z > hi.y ) return ImVec4(dim.x,dim.y,hi.z,dim.w);
        return dim;
    };

    const ImVec4 cyan    = ImVec4(000/255.f, 192/255.f, 255/255.f, 1.00f);
    const ImVec4 red     = ImVec4(240/255.f, 000/255.f,  16/255.f, 1.00f);
    const ImVec4 yellow  = ImVec4(240/255.f, 210/255.f, 000/255.f, 1.00f);
    const ImVec4 orange  = ImVec4(255/255.f, 144/255.f, 000/255.f, 1.00f);
    const ImVec4 lime    = ImVec4(192/255.f, 255/255.f, 000/255.f, 1.00f);
    const ImVec4 aqua    = ImVec4(000/255.f, 255/255.f, 192/255.f, 1.00f);
    const ImVec4 magenta = ImVec4(255/255.f, 000/255.f,  88/255.f, 1.00f);
    const ImVec4 purple  = ImVec4(192/255.f, 000/255.f, 255/255.f, 1.00f);

    ImVec4 alt = cyan;
    /**/ if( alt07 == 0 || alt07 == 'C' ) alt = cyan;
    else if( alt07 == 1 || alt07 == 'R' ) alt = red;
    else if( alt07 == 2 || alt07 == 'Y' ) alt = yellow;
    else if( alt07 == 3 || alt07 == 'O' ) alt = orange;
    else if( alt07 == 4 || alt07 == 'L' ) alt = lime;
    else if( alt07 == 5 || alt07 == 'A' ) alt = aqua;
    else if( alt07 == 6 || alt07 == 'M' ) alt = magenta;
    else if( alt07 == 7 || alt07 == 'P' ) alt = purple;
    if( lit01 ) alt = dim(alt);

    ImVec4 hi = cyan, lo = dim(cyan);
    /**/ if( hue07 == 0 || hue07 == 'C' ) lo = dim( hi = cyan );
    else if( hue07 == 1 || hue07 == 'R' ) lo = dim( hi = red );
    else if( hue07 == 2 || hue07 == 'Y' ) lo = dim( hi = yellow );
    else if( hue07 == 3 || hue07 == 'O' ) lo = dim( hi = orange );
    else if( hue07 == 4 || hue07 == 'L' ) lo = dim( hi = lime );
    else if( hue07 == 5 || hue07 == 'A' ) lo = dim( hi = aqua );
    else if( hue07 == 6 || hue07 == 'M' ) lo = dim( hi = magenta );
    else if( hue07 == 7 || hue07 == 'P' ) lo = dim( hi = purple );
//    if( lit01 ) { ImVec4 tmp = hi; hi = lo; lo = lit(tmp); }

    ImVec4 nav = orange;
    /**/ if( nav07 == 0 || nav07 == 'C' ) nav = cyan;
    else if( nav07 == 1 || nav07 == 'R' ) nav = red;
    else if( nav07 == 2 || nav07 == 'Y' ) nav = yellow;
    else if( nav07 == 3 || nav07 == 'O' ) nav = orange;
    else if( nav07 == 4 || nav07 == 'L' ) nav = lime;
    else if( nav07 == 5 || nav07 == 'A' ) nav = aqua;
    else if( nav07 == 6 || nav07 == 'M' ) nav = magenta;
    else if( nav07 == 7 || nav07 == 'P' ) nav = purple;
    if( lit01 ) nav = dim(nav);

    const ImVec4
    link  = ImVec4(0.26f, 0.59f, 0.98f, 1.00f),
    grey0 = ImVec4(0.04f, 0.05f, 0.07f, 1.00f),
    grey1 = ImVec4(0.08f, 0.09f, 0.11f, 1.00f),
    grey2 = ImVec4(0.10f, 0.11f, 0.13f, 1.00f),
    grey3 = ImVec4(0.12f, 0.13f, 0.15f, 1.00f),
    grey4 = ImVec4(0.16f, 0.17f, 0.19f, 1.00f),
    grey5 = ImVec4(0.18f, 0.19f, 0.21f, 1.00f);

    #define Luma(v,a) ImVec4((v)/100.f,(v)/100.f,(v)/100.f,(a)/100.f)

    style.Colors[ImGuiCol_Text]                      = Luma(100,100);
    style.Colors[ImGuiCol_TextDisabled]              = Luma(39,100);
    style.Colors[ImGuiCol_WindowBg]                  = grey1;
    style.Colors[ImGuiCol_ChildBg]                   = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_PopupBg]                   = grey1;
    style.Colors[ImGuiCol_Border]                    = grey4;
    style.Colors[ImGuiCol_BorderShadow]              = grey1;
    style.Colors[ImGuiCol_FrameBg]                   = ImVec4(0.11f, 0.13f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]            = grey4;
    style.Colors[ImGuiCol_FrameBgActive]             = grey4;
    style.Colors[ImGuiCol_TitleBg]                   = grey0;
    style.Colors[ImGuiCol_TitleBgActive]             = grey0;
    style.Colors[ImGuiCol_TitleBgCollapsed]          = grey1;
    style.Colors[ImGuiCol_MenuBarBg]                 = grey2;
    style.Colors[ImGuiCol_ScrollbarBg]               = grey0;
    style.Colors[ImGuiCol_ScrollbarGrab]             = grey3;
    style.Colors[ImGuiCol_ScrollbarGrabHovered]      = lo;
    style.Colors[ImGuiCol_ScrollbarGrabActive]       = hi;
    style.Colors[ImGuiCol_CheckMark]                 = alt;
    style.Colors[ImGuiCol_SliderGrab]                = lo;
    style.Colors[ImGuiCol_SliderGrabActive]          = hi;
    style.Colors[ImGuiCol_Button]                    = ImVec4(0.10f, 0.11f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]             = lo;
    style.Colors[ImGuiCol_ButtonActive]              = grey5;
    style.Colors[ImGuiCol_Header]                    = grey3;
    style.Colors[ImGuiCol_HeaderHovered]             = lo;
    style.Colors[ImGuiCol_HeaderActive]              = hi;
    style.Colors[ImGuiCol_Separator]                 = ImVec4(0.13f, 0.15f, 0.19f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered]          = lo;
    style.Colors[ImGuiCol_SeparatorActive]           = hi;
    style.Colors[ImGuiCol_ResizeGrip]                = Luma(15,100);
    style.Colors[ImGuiCol_ResizeGripHovered]         = lo;
    style.Colors[ImGuiCol_ResizeGripActive]          = hi;
    style.Colors[ImGuiCol_InputTextCursor]           = Luma(100,100);
    style.Colors[ImGuiCol_TabHovered]                = grey3;
    style.Colors[ImGuiCol_Tab]                       = grey1;
    style.Colors[ImGuiCol_TabSelected]               = grey3;
    style.Colors[ImGuiCol_TabSelectedOverline]       = hi;
    style.Colors[ImGuiCol_TabDimmed]                 = grey1;
    style.Colors[ImGuiCol_TabDimmedSelected]         = grey1;
    style.Colors[ImGuiCol_TabDimmedSelectedOverline] = lo;
    style.Colors[ImGuiCol_DockingPreview]            = grey1;
    style.Colors[ImGuiCol_DockingEmptyBg]            = Luma(20,100);
    style.Colors[ImGuiCol_PlotLines]                 = grey5;
    style.Colors[ImGuiCol_PlotLinesHovered]          = lo;
    style.Colors[ImGuiCol_PlotHistogram]             = grey5;
    style.Colors[ImGuiCol_PlotHistogramHovered]      = lo;
    style.Colors[ImGuiCol_TableHeaderBg]             = grey0;
    style.Colors[ImGuiCol_TableBorderStrong]         = grey0;
    style.Colors[ImGuiCol_TableBorderLight]          = grey0;
    style.Colors[ImGuiCol_TableRowBg]                = grey3;
    style.Colors[ImGuiCol_TableRowBgAlt]             = grey2;
    style.Colors[ImGuiCol_TextLink]                  = link;
    style.Colors[ImGuiCol_TextSelectedBg]            = Luma(39,100);
    style.Colors[ImGuiCol_TreeLines]                 = Luma(39,100);
    style.Colors[ImGuiCol_DragDropTarget]            = nav;
    style.Colors[ImGuiCol_NavCursor]                 = nav;
    style.Colors[ImGuiCol_NavWindowingHighlight]     = lo;
    style.Colors[ImGuiCol_NavWindowingDimBg]         = Luma(0,63);
    style.Colors[ImGuiCol_ModalWindowDimBg]          = Luma(0,63);

    if( lit01 ) {
        for(int i = 0; i < ImGuiCol_COUNT; i++) {
            float H, S, V;
            ImVec4& col = style.Colors[i];
            ImGui::ColorConvertRGBtoHSV( col.x, col.y, col.z, H, S, V );
            if( S < 0.5 ) V = 1.0 - V, S *= 0.15;
            ImGui::ColorConvertHSVtoRGB( H, S, V, col.x, col.y, col.z );
        }
    }

    #undef Luma
    return 0;
}

extern "C"
int igThemeV3_(const char *theme) {
    const char *colors = "CRYOLAMP";
    int hue07 = theme[0] >= 'A' ? strchr(colors, theme[0]) - colors : theme[0] - '0';
    int alt07 = theme[1] >= 'A' ? strchr(colors, theme[1]) - colors : theme[1] - '0';
    int nav07 = theme[2] >= 'A' ? strchr(colors, theme[2]) - colors : theme[2] - '0';
    int lit01 = theme[3] - '0';
    int compact01 = theme[4] - '0';
    int borders01 = theme[5] - '0';
    int shape0123 = theme[6] - '0';
    return igThemeV3(hue07, alt07, nav07, lit01, compact01, borders01, shape0123);
}

extern "C"
int igGetHoveredWindowID(void) {
    auto *win = ImGui::GetCurrentContext()->HoveredWindow;
    return win ? win->ID : 0;
}

#endif
