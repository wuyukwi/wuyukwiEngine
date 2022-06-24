/*********************************************************************************

  *FileName: gameEngine.cpp
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/19

**********************************************************************************/

#include "gameEngine.h"
#include "main.h"

#pragma comment(lib, "Engine/lib/Engine.lib")
#define SOUNDS_PATH ("sounds/")
#define SOUNDS_FILE ("sounds/soundsList.txt")

CRenderInterface* g_Render = nullptr;
CInputInterface* g_Input = nullptr;
CSoundInterface* g_Sound = nullptr;
int g_arialID = -1;

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

    g_Render->SetClearCol(1.0f, 1.0f, 1.0f, 1.0f);

    if (!g_Render->CreateText("Arial", 0, true, 18, g_arialID))
    {
        MessageBox(g_hwnd, "Text 初期化失敗しました。", "エラー", MB_OK);
        return false;
    }

    if (!CreateDIInput(&g_Input, g_hwnd, g_hinstance, false) || !g_Input->Initialize())
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
// 音声初期化関数
//=========================================
bool LoadSoundScript()
{
    g_Sound->Initialize(SOUNDS_PATH);
    g_pScript->LoadScriptFile(SOUNDS_FILE);

    int repeats = 0;
    int id = 0;

    // ループして、指定された各コマンドを実行します。
    for (int i = 0; i < g_pScript->GetTotalLines(); i++)
    {
        char fileName[MAX_COMMAND_SIZE];
        char command[MAX_COMMAND_SIZE];
        g_pScript->ParseCommand(command);

        if (_stricmp(command, "MENU_SOUND") == 0)
        {
            g_pScript->ParseStringParam(fileName);
            repeats = g_pScript->ParseIntParam();
            g_Sound->AddSound(fileName, repeats, command);
        }
        else if (_stricmp(command, "SHOT_SOUND") == 0)
        {
            g_pScript->ParseStringParam(fileName);
            repeats = g_pScript->ParseIntParam();
            g_Sound->AddSound(fileName, repeats, command);
        }

        g_pScript->MoveToNextLine();
    }

    // すべてのリソースを解放します。このスクリプトは、初期化中にのみ実行されるように設計されています。
    g_pScript->Shutdown();
    return true;
}