/********************************************************************************* 

  *FileName: main.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/02

**********************************************************************************/ 


#include "main.h"

// グローバル変数
HWND g_hwnd;
CRenderInterface* g_Render = NULL;

// GUI ids.
int g_mainGui = -1;
int g_startGui = -1;
int g_creditsGui = -1;
int g_currentGUI = GUI_MAIN_SCREEN;

// Font id.
int g_arialID = -1;

// Temp Mouse state information.
bool LMBDown = false;
int mouseX = 0, mouseY = 0;

// imgui_impl_win32.cppからの前方宣言メッセージハンドラ
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI MsgProc(HWND hd, UINT msg, WPARAM wp, LPARAM lp)
{
    //メッセージをimguiに渡す
    if (ImGui_ImplWin32_WndProcHandler(hd, msg, wp, lp))
        return true;

    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);// ウィンドウ破棄のメッセージ
		// WM_QUITメッセージを送る
        return 0;
        break;

    case WM_KEYUP:// [ESC]キーが押された
			// ウィンドウを破棄する(WM_DESTOROYメッセージを送る)
        if (wp == VK_ESCAPE) 
            PostQuitMessage(0);
        break;

    case WM_LBUTTONDOWN:
        LMBDown = true;
        break;

    case WM_LBUTTONUP:
        LMBDown = false;
        break;

    case WM_MOUSEMOVE:
        mouseY = HIWORD(lp);
        mouseX = LOWORD(lp);
        break;
    }

    return DefWindowProc(hd, msg, wp, lp);// 規定の処理を返す
}

//int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdLine, int cmdShow)
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev, _In_ LPSTR cmdLine, _In_ int cmdShow)
{
    // アプリケーションウィンドウ作成
    ImGui_ImplWin32_EnableDpiAwareness();

    // ウインドウクラスの構造体の初期化
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);					// WNDCLASSEXのメモリサイズ
    wcex.style = CS_CLASSDC;							// ウインドウのスタイル
    wcex.lpfnWndProc = MsgProc;						    // ウインドウプロシージャ
    wcex.cbClsExtra = 0;								// ０にする (通常は使用しない)
    wcex.cbWndExtra = 0;								// ０にする (通常は使用しない)
    wcex.hInstance = GetModuleHandle(NULL);				// インスタンスハンドル
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// タスクバーのアイコン
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			// マウスカーソル
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);// クライアント領域の背景色
    wcex.lpszMenuName = NULL;							// メニューバー
    wcex.lpszClassName = WINDOW_CLASS;				// ウインドウクラスの名前
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wcex);


    // ウインドウを作成
    if (FULLSCREEN)
    {
        g_hwnd = CreateWindowEx(
            0,								// 拡張ウインドウスタイル
            WINDOW_CLASS,		            // ウインドウスタイルの名前
            WINDOW_NAME,		            // ウインドウの名前
            WS_POPUP | WS_SYSMENU | WS_VISIBLE,	// ウインドウスタイル
            0,					            // ウインドウの左上X座標
            0,					            // 　　〃　　の左上Y座標
            WIN_WIDTH,		                // 　　〃　　の幅
            WIN_HEIGHT,		                // 　　〃　　の高さ
            NULL,							// 親ウインドウのハンドル
            NULL,							// メニューハンドルまたは子ウインドウID
            instance,				                // インスタンスハンドル
            NULL);							// ウインドウ作成データ

    }
    else
    {
        // クライアント領域を指定のサイズに調整
        RECT rect = { 0, 0, WIN_WIDTH, WIN_HEIGHT };		// 画面サイズの構造体
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

        g_hwnd = CreateWindowEx(
            0,								// 拡張ウインドウスタイル
            WINDOW_CLASS,		            // ウインドウスタイルの名前
            WINDOW_NAME,		            // ウインドウの名前
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// ウインドウスタイル
            0,					            // ウインドウの左上X座標
            0,					            // 　　〃　　の左上Y座標
            (rect.right - rect.left),		//幅
            (rect.bottom - rect.top),		//高さ
            NULL,							// 親ウインドウのハンドル
            NULL,							// メニューハンドルまたは子ウインドウID
            instance,				                // インスタンスハンドル
            NULL);							// ウインドウ作成データ

    }

    if (g_hwnd)
    {
        // ウィンドウの表示
        ShowWindow(g_hwnd, SW_SHOWDEFAULT);
        UpdateWindow(g_hwnd);
    }


    // エンジンの初期化
    if (InitializeEngine())
    {
        // ゲームの初期化
        if (GameInitialize())
        {
            // メッセージループ
            MSG msg;
            ZeroMemory(&msg, sizeof(msg));

            // マウス位置初期化
            SetCursorPos(0, 0);

            // windowsの処理
            while (msg.message != WM_QUIT)
            {
                if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    // メッセージの設定
                    TranslateMessage(&msg);	// 仮想メッセージを文字メッセージへ変換
                    DispatchMessage(&msg);	// ウィンドウプロシージャへメッセージを送信
                }
                else
                {
                    // ゲームループ
                    GameLoop();
                }
            }
        }
    }

    // 全てのリソースをリリース
    GameShutdown();
    ShutdownEngine();

    UnregisterClass(WINDOW_CLASS, wcex.hInstance);
    return true;
}

//=========================================
// エンジン初期化
//=========================================
bool InitializeEngine()
{
    if (!CreateD3DRenderer(&g_Render)) 
        return false;

    if (!g_Render->Initialize(WIN_WIDTH, WIN_HEIGHT,g_hwnd, FULLSCREEN)) 
        return false;

    g_Render->SetClearCol(1.0f, 1.0f, 1.0f,1.0f);

    if(!g_Render->CreateText("Arial",0,true,18,g_arialID))
        return false;

    return true;
}

//=========================================
// エンジン終了
//=========================================
void ShutdownEngine()
{

    // imgui終了処理
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // エンジン終了処理
    if (g_Render)
    {
        g_Render->Shutdown();
        delete g_Render;
        g_Render = NULL;
    }

}

//=========================================
// ゲーム初期化
//=========================================
bool GameInitialize()
{
    if (!InitMainMenu())
        return false;

    return true;
}

//=========================================
// ゲームループ
//=========================================
void GameLoop()
{
    if (!g_Render) 
        return;

    // imguiフレーム作成
    g_Render->StartImGuiFrame();

    // 描画開始
    g_Render->StartRender(1, 1, 0);

    // メニューの描画
    if (g_currentGUI == GUI_MAIN_SCREEN)
        g_Render->ProcessGUI(g_mainGui, LMBDown, mouseX, mouseY, MainMenuCallback);
    else if (g_currentGUI == GUI_START_SCREEN)
        g_Render->ProcessGUI(g_startGui, LMBDown, mouseX, mouseY, MainMenuCallback);
    else if (g_currentGUI == GUI_CREDITS_SCREEN)
        g_Render->ProcessGUI(g_creditsGui, LMBDown, mouseX, mouseY, MainMenuCallback);

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    //MainMenuRender();
     
    // 描画終了
    g_Render->EndRendering();
}

//=========================================
// ゲーム終了
//=========================================
void GameShutdown()
{

}

// メニューの初期化
bool InitMainMenu()
{
    if (!g_Render->CreateGUI(g_mainGui)) return false;
    if (!g_Render->CreateGUI(g_startGui)) return false;
    if (!g_Render->CreateGUI(g_creditsGui)) return false;

    // メニュー背景を読み込む
    if (!g_Render->AddGUIBackdrop(g_mainGui, "Engine/menu/mainMenu.jpg"))
        return false;
    // スタート画面を読み込む
    if (!g_Render->AddGUIBackdrop(g_startGui, "Engine/menu/startMenu.jpg"))
        return false;
    // スタート画面を読み込む
    if (!g_Render->AddGUIBackdrop(g_creditsGui, "Engine/menu/creditsMenu.jpg"))
        return false;

    // メニュー文字列を追加
    if (!g_Render->AddGUIStaticText(g_mainGui, STATIC_TEXT_ID, "Version: 1.0",
        int(WIN_WIDTH * 0.85f), int(WIN_HEIGHT * 0.05f),
        COLOR_ARGB(255, 255, 255, 255), g_arialID))
    {
        return false;
    }

    // メニューボタンを追加
    if (!g_Render->AddGUIButton(g_mainGui, BUTTON_START_ID,
        int(WIN_WIDTH * 0.05f), int(WIN_HEIGHT * 0.4f),
        "Engine/menu/startUp.png", "Engine/menu/startOver.png", "Engine/menu/startDown.png"))
    {
        return false;
    }
    if (!g_Render->AddGUIButton(g_mainGui, BUTTON_CREDITS_ID,
        int(WIN_WIDTH * 0.05f), int(WIN_HEIGHT * 0.5f),
        "Engine/menu/creditsUp.png", "Engine/menu/creditsOver.png", "Engine/menu/creditsDown.png"))
    {
        return false;
    }
    if (!g_Render->AddGUIButton(g_mainGui, BUTTON_QUIT_ID,
        int(WIN_WIDTH * 0.05f), int(WIN_HEIGHT * 0.6f),
        "Engine/menu/quitUp.png", "Engine/menu/quitOver.png", "Engine/menu/quitDown.png"))
    {
        return false;
    }

    if (!g_Render->AddGUIButton(g_startGui, BUTTON_LEVEL_1_ID,
        int(WIN_WIDTH * 0.1f), int(WIN_HEIGHT * 0.15f),
        "Engine/menu/level1Up.png", "Engine/menu/level1Over.png", "Engine/menu/level1Down.png"))
    {
        return false;
    }

    if (!g_Render->AddGUIButton(g_startGui, BUTTON_BACK_ID,
        int(WIN_WIDTH * 0.1f), int(WIN_HEIGHT * 0.8f),
        "Engine/menu/backUp.png", "Engine/menu/backOver.png", "Engine/menu/backDown.png"))
    {
        return false;
    }     
    if (!g_Render->AddGUIButton(g_creditsGui, BUTTON_BACK_ID,
        int(WIN_WIDTH * 0.1f), int(WIN_HEIGHT * 0.8f),
        "Engine/menu/backUp.png", "Engine/menu/backOver.png", "Engine/menu/backDown.png"))
    {
        return false;
    }
}
void MainMenuRender()
{

}

void MainMenuCallback(int id, size_t state)
{
    switch (id)
    {
    case BUTTON_START_ID:
        if (state == UGP_BUTTON_DOWN)
            g_currentGUI = GUI_START_SCREEN;
        break;
    case BUTTON_CREDITS_ID:
        if (state == UGP_BUTTON_DOWN)
            g_currentGUI = GUI_CREDITS_SCREEN;
        break;
    case BUTTON_BACK_ID:
        if (state == UGP_BUTTON_DOWN)
            g_currentGUI = GUI_MAIN_SCREEN;
        break;
    case BUTTON_QUIT_ID:
        if (state == UGP_BUTTON_DOWN)
            PostQuitMessage(0);
        break;
    default:
        break;
    }
}