/********************************************************************************* 

  *FileName: main.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/02

**********************************************************************************/ 


#include "main.h"

#pragma comment(lib, "winmm.lib")

// 設定ファイルパス
#define MAIN_SETTING_FILE   ("Engine/Script/setting.txt")


// グローバル変数
HWND g_hwnd=NULL;
HINSTANCE g_hinstance=NULL;
CScript* g_pScript = nullptr;

// 設定保存用変数
char g_wClass[MAX_COMMAND_SIZE];// ウインドウクラスの名前
char g_wName[MAX_COMMAND_SIZE]; // ウインドウクラスの名前（キャプションに表示）
int g_width = 0;      // ウィンドウの幅
int g_height = 0;   // ウィンドウの高さ
bool g_fullScreen = false;     //全画面
int g_multiSample = 0;

// imgui_impl_win32.cppからの前方宣言メッセージハンドラ
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI MsgProc(HWND hd, UINT msg, WPARAM wp, LPARAM lp)
{
    //メッセージをimguiに渡す
    if (ImGui_ImplWin32_WndProcHandler(hd, msg, wp, lp))
        return true;

    switch (msg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);// ウィンドウ破棄のメッセージ
		// WM_QUITメッセージを送る
        return 0;
        break;
    }

    return DefWindowProc(hd, msg, wp, lp);// 規定の処理を返す
}

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev, _In_ LPSTR cmdLine, _In_ int cmdShow)
{
    // アプリケーションウィンドウ作成
    ImGui_ImplWin32_EnableDpiAwareness();
    
    // 設定スクリプトファイルを読み込み
    LoadScript(MAIN_SETTING_FILE);

    // ウインドウクラスの構造体の初期化
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
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
    wcex.lpszClassName = g_wClass;				// ウインドウクラスの名前
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wcex);


    // ウインドウを作成
    if (g_fullScreen)
    {
        g_hwnd = CreateWindowEx(
            0,								// 拡張ウインドウスタイル
            g_wClass,		            // ウインドウスタイルの名前
            g_wName,		            // ウインドウの名前
            WS_POPUP | WS_SYSMENU | WS_VISIBLE,	// ウインドウスタイル
            0,					            // ウインドウの左上X座標
            0,					            // 　　〃　　の左上Y座標
            g_width,		                // 　　〃　　の幅
            g_height,		                // 　　〃　　の高さ
            NULL,							// 親ウインドウのハンドル
            NULL,							// メニューハンドルまたは子ウインドウID
            instance,				                // インスタンスハンドル
            NULL);							// ウインドウ作成データ

    }
    else
    {
        // クライアント領域を指定のサイズに調整
        RECT rect = { 0, 0, g_width, g_height };		// 画面サイズの構造体
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

        g_hwnd = CreateWindowEx(
            0,								// 拡張ウインドウスタイル
            g_wClass,		            // ウインドウスタイルの名前
            g_wName,		            // ウインドウの名前
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

    g_hinstance = wcex.hInstance;

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
    g_pScript->Shutdown();

    UnregisterClass(g_wClass, wcex.hInstance);
    return true;
}


//=========================================
// スクリプトロード関数
//=========================================
bool LoadScript(const char* filename)
{
    g_pScript = new CScript{};

    if (!g_pScript->LoadScriptFile(filename))
        return false;

    char command[MAX_COMMAND_SIZE];

    // ループして、指定された各コマンドを実行します。
    for (int i = 0; i < g_pScript->GetTotalLines(); i++)
    {
        g_pScript->ParseCommand(command);

        if (_stricmp(command, "WINDOW_CLASS") == 0)
        {
            g_pScript->ParseStringParam(g_wClass);
        }
        else if (_stricmp(command, "WINDOW_NAME") == 0)
        {
            g_pScript->ParseStringParam(g_wName);
        }
        else if (_stricmp(command, "WIN_WIDTH") == 0)
        {
            g_width = g_pScript->ParseIntParam();
        }
        else if (_stricmp(command, "WIN_HEIGHT") == 0)
        {
            g_height = g_pScript->ParseIntParam();
        }
        else if (_stricmp(command, "FULLSCREEN") == 0)
        {
            g_fullScreen = g_pScript->ParseBoolParam();
        }
        else if (_stricmp(command, "MULTI_SAMPLE") == 0)
        {
            g_multiSample = g_pScript->ParseIntParam();
        }
        else if (_stricmp(command, "#") == 0)
        {
            // 何にもしません
        }
        else if (_stricmp(command, "\0") == 0)
        {
            // 何にもしません
        }
        else
        {
            // ここでは、無効なコマンド（構文エラー）を処理できます。
            char err[64];

            sprintf(err, "Error in script on line %d.",
                g_pScript->GetCurrentLineNum() + 1);
            MessageBox(NULL, err, "Error", MB_OK);

            g_pScript->Shutdown();
            return false;
        }

        // スクリプトの次の行に移動します。
        g_pScript->MoveToNextLine();
    }

    // すべてのリソースを解放します。このスクリプトは、初期化中にのみ実行されるように設計されています。
    g_pScript->Shutdown();

    return true;
}






