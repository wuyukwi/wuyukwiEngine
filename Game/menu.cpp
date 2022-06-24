/*********************************************************************************

  *FileName: gameEngine.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/19

**********************************************************************************/

#include "menu.h"
#include "main.h"

// GUI ids.
int g_mainGui = -1;
int g_startGui = -1;
int g_creditsGui = -1;
int g_loadingGui = -1;
int g_currentGUI = GUI_MAIN_SCREEN;

// Sound id.
int g_menuSound = -1;

//=========================================
// メニューの初期化
//=========================================
bool InitializeMainMenu()
{
    if (!g_Render->CreateGUI(g_mainGui)) return false;
    if (!g_Render->CreateGUI(g_startGui)) return false;
    if (!g_Render->CreateGUI(g_creditsGui)) return false;
    if (!g_Render->CreateGUI(g_loadingGui)) return false;

    // メニュー背景を読み込む
    if (!g_Render->AddGUIBackdrop(g_mainGui, "menu/mainMenu.jpg"))
        return false;
    // スタート画面を読み込む
    if (!g_Render->AddGUIBackdrop(g_startGui, "menu/startMenu.jpg"))
        return false;
    // クレジット画面を読み込む
    if (!g_Render->AddGUIBackdrop(g_creditsGui, "menu/creditsMenu.jpg"))
        return false;
    // ロード画面を読み込む
    if (!g_Render->AddGUIBackdrop(g_loadingGui, "menu/loadingMenu.jpg"))
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
        "menu/startUp.png", "menu/startOver.png", "menu/startDown.png"))
    {
        return false;
    }
    if (!g_Render->AddGUIButton(g_mainGui, BUTTON_CREDITS_ID,
        int(g_width * 0.05f), int(g_height * 0.5f),
        "menu/creditsUp.png", "menu/creditsOver.png", "menu/creditsDown.png"))
    {
        return false;
    }
    if (!g_Render->AddGUIButton(g_mainGui, BUTTON_QUIT_ID,
        int(g_width * 0.05f), int(g_height * 0.6f),
        "menu/quitUp.png", "menu/quitOver.png", "menu/quitDown.png"))
    {
        return false;
    }

    if (!g_Render->AddGUIButton(g_startGui, BUTTON_LEVEL_1_ID,
        int(g_width * 0.1f), int(g_height * 0.15f),
        "menu/level1Up.png", "menu/level1Over.png", "menu/level1Down.png"))
    {
        return false;
    }

    if (!g_Render->AddGUIButton(g_startGui, BUTTON_BACK_ID,
        int(g_width * 0.1f), int(g_height * 0.8f),
        "menu/backUp.png", "menu/backOver.png", "menu/backDown.png"))
    {
        return false;
    }
    if (!g_Render->AddGUIButton(g_creditsGui, BUTTON_BACK_ID,
        int(g_width * 0.1f), int(g_height * 0.8f),
        "menu/backUp.png", "menu/backOver.png", "menu/backDown.png"))
    {
        return false;
    }

    g_Sound->Play(0);

    return true;
}

//=========================================
// メニューの描画
//=========================================
void MainMenuRender()
{
    // メニューの描画
    switch (g_currentGUI)
    {
    case GUI_MAIN_SCREEN:
        g_Render->ProcessGUI(g_mainGui, g_LMBDown, g_mouseX, g_mouseY, MainMenuCallback);
        break;

    case GUI_START_SCREEN:
        g_Render->ProcessGUI(g_startGui, g_LMBDown, g_mouseX, g_mouseY, MainMenuCallback);
        break;

    case GUI_CREDITS_SCREEN:
        g_Render->ProcessGUI(g_creditsGui, g_LMBDown, g_mouseX, g_mouseY, MainMenuCallback);
        break;

    case GUI_LOADING_SCREEN:
        g_Render->ProcessGUI(g_loadingGui, g_LMBDown, g_mouseX, g_mouseY, MainMenuCallback);
        break;

    default:
        break;
    }
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

    case BUTTON_QUIT_ID:
        if (state == UGP_BUTTON_DOWN)
        {
            GameReleaseAll();
            PostQuitMessage(0);
        }
        break;

    case BUTTON_BACK_ID:
        if (state == UGP_BUTTON_DOWN)
        {
            g_currentGUI = GUI_MAIN_SCREEN;
        }
        break;

    case BUTTON_LEVEL_1_ID:
        if (state == UGP_BUTTON_DOWN)
        {
            g_currentGUI = GUI_LOADING_SCREEN;
            g_gameState = GAME_STATE::GS_LEVEL_1_SWITCH;
        }
        break;

    default:
        break;
    }
}