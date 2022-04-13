/********************************************************************************* 

  *FileName: main.h  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/02

**********************************************************************************/  


#ifndef MAIN_H_
#define MAIN_H_

#include "Engine/engine.h"

//------------------------------------
// ライブラリのリンク
//------------------------------------
#pragma comment(lib, "Engine/lib/Engine.lib")

#define MAIN_SETTING_FILE   ("Engine/Script/setting.txt")

#define SOUNDS_PATH ("sounds/")
#define SOUNDS_FILE ("sounds/soundsList.txt")

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
bool InitializeEngine();
void ShutdownEngine();

//-----------------------------------------
// スクリプト関数
//-----------------------------------------
bool LoadScript(const char* filename);

//-----------------------------------------
// メニュー関数
//-----------------------------------------
bool InitMainMenu();
void MainMenuRender();
void MainMenuCallback(int id, size_t state);
void fpsRender();


//-----------------------------------------
// 音声関数
//-----------------------------------------
bool LoadSoundScript();

//-----------------------------------------
// ゲーム関数
//-----------------------------------------
bool GameInitialize();
void GameLoop();
void GameShutdown();

//-----------------------------------------
// インプット関数
//-----------------------------------------
void ProcessInput();

// メインメニュー
#define GUI_MAIN_SCREEN       (1)
#define GUI_START_SCREEN      (2)
#define GUI_CREDITS_SCREEN    (3)

// GUIコントローラー
#define STATIC_TEXT_ID     (1)
#define BUTTON_START_ID    (2)
#define BUTTON_CREDITS_ID  (3)
#define BUTTON_QUIT_ID     (4)
#define BUTTON_BACK_ID     (5)
#define BUTTON_LEVEL_1_ID  (6)


#endif // MAIN_H_