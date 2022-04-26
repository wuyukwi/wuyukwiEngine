/*********************************************************************************

  *FileName: game.h
            ƒRƒE  ƒLƒKƒN
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/19

**********************************************************************************/

#include "game.h"
#include "main.h"
#include "Engine/defines.h"

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

//=========================================
// ƒQ[ƒ€‰Šú‰»
//=========================================
bool GameInitialize()
{
    g_Render->SetClearCol(0.0f, 0.0f, 0.0f, 1.0f);
    if (!InitializeMainMenu())
        return false;

    return true;
}



//=========================================
// ƒQ[ƒ€ƒ‹[ƒv
//=========================================
void GameLoop()
{
    if (!g_Render) 
        return;

    // imguiƒtƒŒ[ƒ€ì¬
    g_Render->StartImGuiFrame();

    // InputXVˆ—
    GameProcessInput();

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("mouseXY :%d,%d,%d",g_Input->GetMousePos(true).x, g_Input->GetMousePos(true).y, g_LMBDown);               // Display some text (you can use a format strings too)
    ImGui::End();

    // •`‰æŠJŽn
    g_Render->StartRender(1, 1, 0);
     
    if (g_gameState == GAME_STATE::GS_LEVEL)
    {
        GameWorldRender();
    }
    else if (g_gameState == GAME_STATE::GS_MENU)
    {
        MainMenuRender();
    }
    else if (g_gameState == GAME_STATE::GS_LEVEL_1_SWITCH)
    {
        MainMenuRender();
        g_Render->EndRendering();
        GameReleaseAll();

        if (GameWorldLoad("maps/level1/level1.lvl"))
        {
            g_gameState = GAME_STATE::GS_LEVEL;
        }
        else
        {
            InitializeMainMenu();
            g_Sound->Play(g_menuSound);
            g_gameState = GAME_STATE::GS_MENU;
            g_currentGUI = GUI_MAIN_SCREEN;
        }
    }

    if (g_gameState == GAME_STATE::GS_LEVEL)
    {
  
    }

    // InGui‚Ì•`‰æ
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    //FPS‚Ì•`‰æ
    fpsRender();

    // •`‰æI—¹
    g_Render->EndRendering();
}

//=========================================
// ƒQ[ƒ€I—¹
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
// fps•`‰æŠÖ”
//=========================================
void fpsRender()
{
    //FPS‚ðŒv‘ª—p•Ï”
    static int fps = 0;
    static float currentTime = 0.0f;
    static float lastTime = 0.0f;
    static float frameTime = 0.0f;

    //FPS‚Ì•`‰æ
    g_Render->DisplayText(g_arialID, 0, 0, COLOR_ARGB(255, 255, 255, 255), g_fpsStr);

    static char s_elapsed[16] = { 0 };
    sprintf(s_elapsed, "elapsed: %f", g_elapsed);
    g_Render->DisplayText(g_arialID, 0, 20, COLOR_ARGB(255, 255, 255, 255), s_elapsed);

    static char timeCount[16] = { 0 };
    sprintf(timeCount, "timeCount: %d", g_timeCount);
    g_Render->DisplayText(g_arialID, 0, 40, COLOR_ARGB(255, 255, 255, 255), timeCount);

    //FPS‚ðŒv‘ª
    currentTime = float(timeGetTime());
    g_elapsed = ((currentTime - lastTime) * 0.001f);
    lastTime = currentTime;

    frameTime += g_elapsed;

    // 1•bŒo‰ß
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
// “ü—ÍŠÖ”
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
    /*ImGui::Text("camera pos :%f,%f,%f", g_camera.m_pos.x, g_camera.m_pos.y, g_camera.m_pos.z);
    ImGui::Text("camera view :%f,%f,%f", g_camera.m_view.x, g_camera.m_view.y, g_camera.m_view.z);
    ImGui::Text("camera up :%f,%f,%f", g_camera.m_up.x, g_camera.m_up.y, g_camera.m_up.z);*/
    static float angle = 0.0f;
    static float cameraRot[3] = { 0.0f };
    if (g_Input->MouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        
    }

    ImGui::SliderFloat3("rot", cameraRot, -1.0f, 1.0f);
    ImGui::SliderFloat("angle", &angle, -3.14f, 3.14f);
    ImGui::End();
    if (g_gameState == GAME_STATE::GS_LEVEL &&g_gameWorld.m_missionStatus == MISSION_ONGOING)
    {
        if (g_Input->KeyDown(DIK_W))
        {
           
         
        
        }

        if (g_Input->KeyDown(DIK_S))
        {
           

  
        }

        g_Input->GetMousePos();

        if (g_Input->KeyDown(DIK_A))
        {
            
        }

        if (g_Input->KeyDown(DIK_D))
        {
            
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
