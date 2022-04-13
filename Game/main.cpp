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
HINSTANCE g_hinstance;
char g_fpsStr[16] = { 0 };
float g_elapsed = 0.0f;
int g_timeCount = 0;
CRenderInterface* g_Render = nullptr;
CInputInterface* g_Input = nullptr;
CSoundInterface* g_Sound = nullptr;

// スクリプトから読み込み用変数
CScript* g_pScript = nullptr;
char g_wClass[MAX_COMMAND_SIZE];// ウインドウクラスの名前
char g_wName[MAX_COMMAND_SIZE]; // ウインドウクラスの名前（キャプションに表示）
int g_width = 0;      // ウィンドウの幅
int g_height = 0;   // ウィンドウの高さ
bool g_fullScreen = false;     //全画面
int g_multiSample = 0;

// GUI ids.
int g_mainGui = -1;
int g_startGui = -1;
int g_creditsGui = -1;
int g_currentGUI = GUI_MAIN_SCREEN;

// Font id.
int g_arialID = -1;

// Temp Mouse state information.
bool g_LMBDown = false;
int g_mouseX = 0, g_mouseY = 0;

int g_menuSound = -1;

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

   // case WM_KEYUP:// [ESC]キーが押された
			//// ウィンドウを破棄する(WM_DESTOROYメッセージを送る)
   //     if (wp == VK_ESCAPE) 
   //         PostQuitMessage(0);
   //     break;

   // case WM_LBUTTONDOWN:
   //     g_LMBDown = true;
   //     break;

   // case WM_LBUTTONUP:
   //     g_LMBDown = false;
   //     break;

   // case WM_MOUSEMOVE:
   //     g_mouseY = HIWORD(lp);
   //     g_mouseX = LOWORD(lp);
   //     break;
    }

    return DefWindowProc(hd, msg, wp, lp);// 規定の処理を返す
}

//int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdLine, int cmdShow)
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev, _In_ LPSTR cmdLine, _In_ int cmdShow)
{
    // アプリケーションウィンドウ作成
    ImGui_ImplWin32_EnableDpiAwareness();
    
    // 設定スクリプトファイルを読み込み
    LoadScript(MAIN_SETTING_FILE);

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

    UnregisterClass(g_wClass, wcex.hInstance);
    return true;
}

//=========================================
// エンジン初期化
//=========================================
bool InitializeEngine()
{
    if (!CreateD3DRenderer(&g_Render)) 
    {
        MessageBox(g_hwnd, "Render 初期化失敗しました。", "エラー", MB_OK);
        return false;
    }

    if (!g_Render->Initialize(g_width, g_height, g_hwnd, g_fullScreen, g_multiSample))
    {
        MessageBox(g_hwnd, "Render 初期化失敗しました。", "エラー", MB_OK);
        return false;
    }

    g_Render->SetClearCol(1.0f, 1.0f, 1.0f,1.0f);

    if(!g_Render->CreateText("Arial",0,true,18,g_arialID))
    {
        MessageBox(g_hwnd, "Text 初期化失敗しました。", "エラー", MB_OK);
        return false;
    }

    if (!CreateDIInput(&g_Input, g_hwnd, g_hinstance, false)|| !g_Input->Initialize())
    {
        MessageBox(g_hwnd, "Input 初期化失敗しました。", "エラー", MB_OK);
        return false;
    }

    if (!CreateDMSound(&g_Sound) || !LoadSoundScript())
    {
        MessageBox(g_hwnd, "Sound 初期化失敗しました。", "エラー", MB_OK);
        return false;
    }

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
        g_Render = nullptr;
    }

    // Input終了処理
    if (g_Input)
    {
        g_Input->Shutdown();
        delete g_Input;
        g_Input = nullptr;
    }

    if (g_Sound)
    {
        g_Sound->Shutdown();
        delete g_Sound;
        g_Sound = nullptr;
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

    // Input更新処理
    ProcessInput();

    // imguiフレーム作成
    g_Render->StartImGuiFrame();

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("mouseXY :%d,%d",g_mouseX,g_mouseY);               // Display some text (you can use a format strings too)
  
    ImGui::End();

    // 描画開始
    g_Render->StartRender(1, 1, 0);

    // メニューの描画
    MainMenuRender();

    // InGuiの描画
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
     
    //FPSの描画
    fpsRender();

    // 描画終了
    g_Render->EndRendering();

}

//=========================================
// ゲーム終了
//=========================================
void GameShutdown()
{

}

//=========================================
// メニューの初期化
//=========================================
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
        int(g_width * 0.85f), int(g_height * 0.05f),
        COLOR_ARGB(255, 255, 255, 255), g_arialID))
    {
        return false;
    }

    // メニューボタンを追加
    if (!g_Render->AddGUIButton(g_mainGui, BUTTON_START_ID,
        int(g_width * 0.05f), int(g_height * 0.4f),
        "Engine/menu/startUp.png", "Engine/menu/startOver.png", "Engine/menu/startDown.png"))
    {
        return false;
    }
    if (!g_Render->AddGUIButton(g_mainGui, BUTTON_CREDITS_ID,
        int(g_width * 0.05f), int(g_height * 0.5f),
        "Engine/menu/creditsUp.png", "Engine/menu/creditsOver.png", "Engine/menu/creditsDown.png"))
    {
        return false;
    }
    if (!g_Render->AddGUIButton(g_mainGui, BUTTON_QUIT_ID,
        int(g_width * 0.05f), int(g_height * 0.6f),
        "Engine/menu/quitUp.png", "Engine/menu/quitOver.png", "Engine/menu/quitDown.png"))
    {
        return false;
    }

    if (!g_Render->AddGUIButton(g_startGui, BUTTON_LEVEL_1_ID,
        int(g_width * 0.1f), int(g_height * 0.15f),
        "Engine/menu/level1Up.png", "Engine/menu/level1Over.png", "Engine/menu/level1Down.png"))
    {
        return false;
    }

    if (!g_Render->AddGUIButton(g_startGui, BUTTON_BACK_ID,
        int(g_width * 0.1f), int(g_height * 0.8f),
        "Engine/menu/backUp.png", "Engine/menu/backOver.png", "Engine/menu/backDown.png"))
    {
        return false;
    }     
    if (!g_Render->AddGUIButton(g_creditsGui, BUTTON_BACK_ID,
        int(g_width * 0.1f), int(g_height * 0.8f),
        "Engine/menu/backUp.png", "Engine/menu/backOver.png", "Engine/menu/backDown.png"))
    {
        return false;
    }

    g_Sound->Play(1);

    return true;
}

//=========================================
// メニューの描画
//=========================================
void MainMenuRender()
{
    // メニューの描画
    if (g_currentGUI == GUI_MAIN_SCREEN)
        g_Render->ProcessGUI(g_mainGui, g_LMBDown, g_mouseX, g_mouseY, MainMenuCallback);
    else if (g_currentGUI == GUI_START_SCREEN)
        g_Render->ProcessGUI(g_startGui, g_LMBDown, g_mouseX, g_mouseY, MainMenuCallback);
    else if (g_currentGUI == GUI_CREDITS_SCREEN)
        g_Render->ProcessGUI(g_creditsGui, g_LMBDown, g_mouseX, g_mouseY, MainMenuCallback);
}

//=========================================
// メニューコールバック関数
//=========================================
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

//=========================================
// fps描画関数
//=========================================
void fpsRender()
{
    //FPSを計測用変数
    static int fps = 0;
    static float currentTime = 0.0f;
    static float lastTime = 0.0f;
    static float frameTime = 0.0f;

    //FPSの描画
    g_Render->DisplayText(g_arialID, 0, 0, COLOR_ARGB(255, 255, 255, 255), g_fpsStr);

    static char cframeTime[16] = { 0 };
    sprintf(cframeTime, "frameTime: %f", frameTime);
    g_Render->DisplayText(g_arialID, 0, 20, COLOR_ARGB(255, 255, 255, 255), cframeTime);

    static char timeCount[16] = { 0 };
    sprintf(timeCount, "timeCount: %d", g_timeCount);
    g_Render->DisplayText(g_arialID, 0, 40, COLOR_ARGB(255, 255, 255, 255), timeCount);

    //FPSを計測
    currentTime = float(GetTickCount());
    g_elapsed = (currentTime - lastTime) * 0.001f;
    lastTime = currentTime;

    frameTime += g_elapsed;

    // 1秒経過
    if (frameTime > 1.0f)
    {
        sprintf(g_fpsStr, "FPS: %d", fps);

        g_timeCount++;
        fps = 0;
        frameTime = 0.0f;
    }
    else
    {
        fps++;
    }
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

//=========================================
// 入力関数
//=========================================
void ProcessInput()
{
    if (!g_Input) 
        return;

    g_Input->UpdateDevices();

    if (g_Input->KeyUp(DIK_ESCAPE))
    {
        switch (g_currentGUI)
        {
        case GUI_MAIN_SCREEN:
            PostQuitMessage(0);
            break;
        default:
            g_currentGUI = GUI_MAIN_SCREEN;
        }
    }

    POINT pos=g_Input->GetMousePos();

    g_mouseX = pos.x;
    g_mouseY = pos.y;

    g_LMBDown = g_Input->MouseButtonDown(MOUSE_LEFT_BUTTON);

}

//=========================================
// 音声初期化関数
//=========================================
bool LoadSoundScript()
{
    g_Sound->Initialize(SOUNDS_PATH);
    g_pScript->LoadScriptFile(SOUNDS_FILE);

    char command[MAX_COMMAND_SIZE];
    char fileName[MAX_COMMAND_SIZE]; 
    int repeats = 0;
    int id = 0;

    // ループして、指定された各コマンドを実行します。
    for (int i = 0; i < g_pScript->GetTotalLines(); i++)
    {
        g_pScript->ParseCommand(command);

    if (_stricmp(command, "COMMAND_BEGIN") == 0)
    {
        g_pScript->ParseSoundFileParan(fileName, &repeats);
        g_Sound->AddSound(fileName,repeats,id);
        id++;
    }
    else if (_stricmp(command, "COMMAND_END") == 0)
    {
 
    }

    }

    // すべてのリソースを解放します。このスクリプトは、初期化中にのみ実行されるように設計されています。
    g_pScript->Shutdown();
    return true;
}