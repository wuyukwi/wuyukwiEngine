/*********************************************************************************

  *FileName: gameEngine.cpp
            �R�E  �L�K�N
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
// �G���W��������
//=========================================
bool InitializeEngine()
{
    if (!CreateD3DRenderer(&g_Render))
    {
        MessageBox(g_hwnd, "Render ���������s���܂����B", "�G���[", MB_OK);
        return false;
    }

    if (!g_Render->Initialize(g_width, g_height, g_hwnd, g_fullScreen, g_multiSample))
    {
        MessageBox(g_hwnd, "Render ���������s���܂����B", "�G���[", MB_OK);
        return false;
    }

    g_Render->SetClearCol(1.0f, 1.0f, 1.0f, 1.0f);

    if (!g_Render->CreateText("Arial", 0, true, 18, g_arialID))
    {
        MessageBox(g_hwnd, "Text ���������s���܂����B", "�G���[", MB_OK);
        return false;
    }

    if (!CreateDIInput(&g_Input, g_hwnd, g_hinstance, false) || !g_Input->Initialize())
    {
        MessageBox(g_hwnd, "Input ���������s���܂����B", "�G���[", MB_OK);
        return false;
    }

    if (!CreateDMSound(&g_Sound) || !LoadSoundScript())
    {
        MessageBox(g_hwnd, "Sound ���������s���܂����B", "�G���[", MB_OK);
        return false;
    }

    return true;
}

//=========================================
// �G���W���I��
//=========================================
void ShutdownEngine()
{

    // imgui�I������
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // �G���W���I������
    if (g_Render)
    {
        g_Render->Shutdown();
        delete g_Render;
        g_Render = nullptr;
    }

    // Input�I������
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
// �����������֐�
//=========================================
bool LoadSoundScript()
{
    g_Sound->Initialize(SOUNDS_PATH);
    g_pScript->LoadScriptFile(SOUNDS_FILE);

    char command[MAX_COMMAND_SIZE];
    char fileName[MAX_COMMAND_SIZE];
    int repeats = 0;
    int id = 0;

    // ���[�v���āA�w�肳�ꂽ�e�R�}���h�����s���܂��B
    for (int i = 0; i < g_pScript->GetTotalLines(); i++)
    {
        g_pScript->ParseCommand(command);
  
        if (_stricmp(command, "MENU_SOUND") == 0)
        {
            g_pScript->ParseStringParam(fileName);
            repeats = g_pScript->ParseIntParam();
            g_Sound->AddSound(fileName, repeats, g_menuSound);
        }
        else if (_stricmp(command, "SHOT_SOUND") == 0)
        {
            g_pScript->ParseStringParam(fileName);
            repeats = g_pScript->ParseIntParam();
            g_Sound->AddSound(fileName, repeats,g_shotSound );
        }

        g_pScript->MoveToNextLine();
    }

    // ���ׂẴ��\�[�X��������܂��B���̃X�N���v�g�́A���������ɂ̂ݎ��s�����悤�ɐ݌v����Ă��܂��B
    g_pScript->Shutdown();
    return true;
}
