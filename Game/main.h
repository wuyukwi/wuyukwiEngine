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

#define WINDOW_CLASS    ("TestGame")// ウインドウクラスの名前
#define WINDOW_NAME     ("Test")// ウインドウクラスの名前（キャプションに表示）
#define WIN_WIDTH       (1280) // ウィンドウの幅
#define WIN_HEIGHT      (720)  // ウィンドウの高さ
#define FULLSCREEN      (0)    //全画面

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
bool InitializeEngine();
void ShutdownEngine();

//-----------------------------------------
// メニュー関数
//-----------------------------------------
bool InitMainMenu();
void MainMenuRender();
void MainMenuCallback(int id, size_t state);

// Main menu defines.
#define GUI_MAIN_SCREEN       (1)
#define GUI_START_SCREEN      (2)
#define GUI_CREDITS_SCREEN    (3)

// ids for our GUI controls.
#define STATIC_TEXT_ID     (1)
#define BUTTON_START_ID    (2)
#define BUTTON_CREDITS_ID  (3)
#define BUTTON_QUIT_ID     (4)
#define BUTTON_BACK_ID     (5)
#define BUTTON_LEVEL_1_ID  (6)

//-----------------------------------------
// ゲーム関数
//-----------------------------------------
bool GameInitialize();
void GameLoop();
void GameShutdown();

#endif // MAIN_H_