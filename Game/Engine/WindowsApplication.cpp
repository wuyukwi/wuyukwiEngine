#include "WindowsApplication.h"
#include "engine.h"

#pragma comment(lib, "winmm.lib")

bool WindowsApplication::m_enableImgui = false;
bool WindowsApplication::m_IsQuit = false;

bool WindowsApplication::Initialize(GfxConfiguration* cfg, bool enable_imgui)
{
    m_Config = cfg;

    if (enable_imgui)
    {
        ImGui_ImplWin32_EnableDpiAwareness();
        m_enableImgui = true;
    }
    else
    {
        m_enableImgui = false;
    }

    // ウインドウクラスの構造体の初期化
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);					// WNDCLASSEXのメモリサイズ
    wcex.style = CS_CLASSDC;							// ウインドウのスタイル
    wcex.lpfnWndProc = WindowProc;						    // ウインドウプロシージャ
    wcex.cbClsExtra = 0;								// ０にする (通常は使用しない)
    wcex.cbWndExtra = 0;								// ０にする (通常は使用しない)
    wcex.hInstance = GetModuleHandle(nullptr);				// インスタンスハンドル
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);		// タスクバーのアイコン
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);			// マウスカーソル
    wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));// クライアント領域の背景色
    wcex.lpszMenuName = nullptr;							// メニューバー
    wcex.lpszClassName = m_Config->appName;				// ウインドウクラスの名前
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassEx(&wcex);

    m_hinstance = wcex.hInstance;

    // ウインドウを作成
    if (m_Config->fullscreen)
    {
        m_hwnd = CreateWindowEx(
            0,								// 拡張ウインドウスタイル
            m_Config->winClassName,		            // ウインドウスタイルの名前
            m_Config->appName,		            // ウインドウの名前
            WS_POPUP | WS_SYSMENU | WS_VISIBLE,	// ウインドウスタイル
            0,					            // ウインドウの左上X座標
            0,					            // 　　〃　　の左上Y座標
            m_Config->screenWidth,		                // 　　〃　　の幅
            m_Config->screenHeight,		                // 　　〃　　の高さ
            nullptr,							// 親ウインドウのハンドル
            nullptr,							// メニューハンドルまたは子ウインドウID
            m_hinstance,				                // インスタンスハンドル
            nullptr);							// ウインドウ作成データ
    }
    else
    {
        // クライアント領域を指定のサイズに調整
        RECT rect = { 0, 0, static_cast<long>(m_Config->screenWidth), static_cast<long>(m_Config->screenHeight) };		// 画面サイズの構造体
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

        m_hwnd = CreateWindowEx(
            0,								// 拡張ウインドウスタイル
            m_Config->winClassName,		            // ウインドウスタイルの名前
            m_Config->appName,		            // ウインドウの名前
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// ウインドウスタイル
            0,					            // ウインドウの左上X座標
            0,					            // 　　〃　　の左上Y座標
            (rect.right - rect.left),		//幅
            (rect.bottom - rect.top),		//高さ
            nullptr,							// 親ウインドウのハンドル
            nullptr,							// メニューハンドルまたは子ウインドウID
            m_hinstance,				                // インスタンスハンドル
            nullptr);							// ウインドウ作成データ
    }


    if (m_hwnd)
    {
        // ウィンドウの表示
        ShowWindow(m_hwnd, SW_SHOWDEFAULT);
        UpdateWindow(m_hwnd);
    }

    return true;
}

void WindowsApplication::Shutdown()
{
    if (m_enableImgui)
    {
        // imgui終了処理
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
}

void WindowsApplication::Tick()
{
    // ウィンドウズメッセージハンドル
    MSG msg;

    // メッセージループ
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        // 仮想メッセージを文字メッセージへ変換
        TranslateMessage(&msg);

        // ウィンドウプロシージャへメッセージを送信
        DispatchMessage(&msg);
    }
}

bool WindowsApplication::IsQuit()
{
    return m_IsQuit;
}


// imgui_impl_win32.cppからの前方宣言メッセージハンドラ
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //メッセージをimguiに渡す
    if (m_enableImgui)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
            return true;
    }

    switch (message)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);// ウィンドウ破棄のメッセージ
        m_IsQuit = true;
        // WM_QUITメッセージを送る
        return 0;
    default: break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);// 規定の処理を返す
}
