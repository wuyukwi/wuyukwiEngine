/*********************************************************************************

  *FileName: game.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/19

**********************************************************************************/

#include "game.h"
#include "main.h"
#include "Engine/defines.h"
#include "Engine/Scene.h"
#include "gameScene.h"

char g_fpsStr[16] = { 0 };
float g_elapsed = 0.0f;
int g_timeCount = 0;

// Camera.
CCamera g_camera;

// Sound id for weapon shot.
int g_shotSound = -1;

// Temp Mouse state information.
bool g_LMBDown = false;
int g_mouseX = 0, g_mouseY = 0;

stGameWorld g_gameWorld;

GAME_STATE g_gameState = GAME_STATE::GS_MENU;

Scene* g_scene;

//=========================================
// ゲーム初期化
//=========================================
bool GameInitialize()
{
    g_Render->SetClearCol(0.0f, 0.0f, 0.0f, 1.0f);
    if (!InitializeMainMenu())
        return false;

    g_scene = new gameScene();
    g_scene->Start();

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

    // Input更新処理
    GameProcessInput();

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("mouseXY :%d,%d,%d", g_Input->GetMousePos(true).x, g_Input->GetMousePos(true).y, g_LMBDown);
    ImGui::End();

    // 描画開始
    g_Render->StartRender(1, 1, 0);

    if (g_gameState == GAME_STATE::GS_LEVEL)
    {
        g_scene->Updata(g_elapsed);
        g_scene->Render();

        //GameWorldRender();

    }
    else if (g_gameState == GAME_STATE::GS_MENU)
    {
        MainMenuRender();
    }
    else if (g_gameState == GAME_STATE::GS_LEVEL_1_SWITCH)
    {
        g_gameState = GAME_STATE::GS_LEVEL;
        /*    MainMenuRender();
            g_Render->EndRendering();
            GameReleaseAll();

            if (GameWorldLoad("maps/level1/level1.lvl"))
            {

            }
            else
            {
                InitializeMainMenu();
                g_Sound->Play(g_menuSound);
                g_gameState = GAME_STATE::GS_MENU;
                g_currentGUI = GUI_MAIN_SCREEN;
            }*/
    }

    if (g_gameState == GAME_STATE::GS_LEVEL)
    {
    }

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
    GameReleaseAll();
}

void GameReleaseAll()
{
    if (!g_Render) return;

    g_Render->ReleaseAllStaticBuffers();
    g_Render->ReleaseAllXModels();
    g_Render->ReleaseAllGUIs();

    g_gameWorld.m_levelID = -1;
    g_gameWorld.m_skyBoxID = -1;

    g_mainGui = -1;
    g_startGui = -1;
    g_creditsGui = -1;
    g_currentGUI = GUI_MAIN_SCREEN;

    if (!g_Sound) return;

    if (g_gameState != GAME_STATE::GS_MENU && g_gameState != GAME_STATE::GS_LEVEL)
        g_Sound->Stop(g_menuSound);
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

    static char s_elapsed[16] = { 0 };
    sprintf(s_elapsed, "elapsed: %f", g_elapsed);
    g_Render->DisplayText(g_arialID, 0, 20, COLOR_ARGB(255, 255, 255, 255), s_elapsed);

    static char timeCount[16] = { 0 };
    sprintf(timeCount, "timeCount: %d", g_timeCount);
    g_Render->DisplayText(g_arialID, 0, 40, COLOR_ARGB(255, 255, 255, 255), timeCount);

    //FPSを計測
    currentTime = float(timeGetTime());
    g_elapsed = ((currentTime - lastTime) * 0.001f);
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
// 入力関数
//=========================================
void GameProcessInput()
{
    if (!g_Input) return;

    // Update all devices.
    g_Input->UpdateDevices();

    // Keyboard Input.
    if (g_Input->KeyUp(DIK_ESCAPE))
    {
        if (g_gameState == GAME_STATE::GS_MENU)
        {
            switch (g_currentGUI)
            {
            case GUI_MAIN_SCREEN:
                PostQuitMessage(0);
                break;

            default:
                g_currentGUI = GUI_MAIN_SCREEN;
                break;
            }
        }
        else if (g_gameState == GAME_STATE::GS_LEVEL)
        {
            GameReleaseAll();

            InitializeMainMenu();
            g_Sound->Play(g_menuSound);
            g_gameState = GAME_STATE::GS_MENU;
            g_currentGUI = GUI_MAIN_SCREEN;
        }
    }

    ImGui::Begin("camera");
    ImGui::Text("camera pos :%f,%f,%f", g_camera.GetCameraPos()->x, g_camera.GetCameraPos()->y, g_camera.GetCameraPos()->z);
    ImGui::Text("camera right :%f,%f,%f", g_camera.GetCameraRight()->x, g_camera.GetCameraRight()->y, g_camera.GetCameraRight()->z);
    ImGui::Text("camera up :%f,%f,%f", g_camera.GetCameraUp()->x, g_camera.GetCameraUp()->y, g_camera.GetCameraUp()->z);
    ImGui::Text("camera look :%f,%f,%f", g_camera.GetCameraLook()->x, g_camera.GetCameraLook()->y, g_camera.GetCameraLook()->z);

    static float angle = 0.0f;
    static float cameraRot[3] = { 0.0f };
    if (g_Input->MouseButtonDown(MOUSE_LEFT_BUTTON))
    {
    }

    ImGui::SliderFloat3("rot", cameraRot, -1.0f, 1.0f);
    ImGui::SliderFloat("angle", &angle, -3.14f, 3.14f);
    ImGui::End();
    if (g_gameState == GAME_STATE::GS_LEVEL && g_gameWorld.m_missionStatus == MISSION_ONGOING)
    {
        if (g_Input->KeyDown(DIK_W))
        {
            g_camera.walk(1.0f);
        }

        if (g_Input->KeyDown(DIK_S))
        {
            g_camera.walk(-1.0f);
        }

        g_Input->GetMousePos();

        if (g_Input->KeyDown(DIK_A))
        {
            g_camera.strafe(-1.0f);
        }

        if (g_Input->KeyDown(DIK_D))
        {
            g_camera.strafe(1.0f);
        }

        if (g_Input->KeyDown(DIK_R))
        {
            g_camera.fly(0.5f);
        }

        if (g_Input->KeyDown(DIK_F))
        {
            g_camera.fly(-0.5f);
        }

        if (g_Input->KeyDown(DIK_UP))
        {
            g_camera.pitch(0.01f);
        }
        if (g_Input->KeyDown(DIK_DOWN))
        {
            g_camera.pitch(-0.01f);
        }
        if (g_Input->KeyDown(DIK_LEFT))
        {
            g_camera.yaw(0.01f);
        }
        if (g_Input->KeyDown(DIK_RIGHT))
        {
            g_camera.yaw(-0.01f);
        }

        if (g_Input->MouseButtonUp(MOUSE_LEFT_BUTTON))
        {
        }
    }

    // Mouse Input
    POINT pos = g_Input->GetMousePos();
    g_mouseX = pos.x;
    g_mouseY = pos.y;

    if (g_Input->MouseButtonDown(MOUSE_LEFT_BUTTON))
        g_LMBDown = true;
    if (!g_Input->MouseButtonDown(MOUSE_LEFT_BUTTON))
        g_LMBDown = false;
}