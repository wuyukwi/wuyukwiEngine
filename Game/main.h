/*********************************************************************************

  *FileName: main.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/02

**********************************************************************************/

#ifndef MAIN_H_
#define MAIN_H_

#include "gameEngine.h"
#include "game.h"
#include "menu.h"
#include "gameLoad.h"


//*****************************************
// グローバル関数宣言
//*****************************************
bool LoadScript(const char* filename);  // スクリプトロード関数

//*****************************************
// グローバル変数宣言
//*****************************************
extern HWND g_hwnd;
extern HINSTANCE g_hinstance;
extern CScript* g_pScript;
extern int g_width;      // ウィンドウの幅
extern int g_height;   // ウィンドウの高さ
extern bool g_fullScreen;     //全画面
extern int g_multiSample;

#endif // MAIN_H_