/*********************************************************************************

  *FileName: gameEngine.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/19

**********************************************************************************/

#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include "Engine/engine.h"

//*****************************************
// グローバル関数宣言
//*****************************************
bool InitializeEngine();
void ShutdownEngine();
bool LoadSoundScript();     //スクリプト関数

//*****************************************
// グローバル変数
//*****************************************
extern CRenderInterface* g_Render;
extern CInputInterface* g_Input;
extern CSoundInterface* g_Sound;
extern int g_arialID;       //フォントid

#endif _GAME_ENGINE_H_