/*********************************************************************************

  *FileName: gameEngine.h
            �R�E  �L�K�N
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/19

**********************************************************************************/


#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include "Engine/engine.h"

//*****************************************
// �O���[�o���֐��錾
//*****************************************
bool InitializeEngine();
void ShutdownEngine();
bool LoadSoundScript();     //�X�N���v�g�֐�

//*****************************************
// �O���[�o���ϐ�
//*****************************************
extern CRenderInterface* g_Render;
extern CInputInterface* g_Input;
extern CSoundInterface* g_Sound;
extern int g_arialID;       //�t�H���gid

#endif _GAME_ENGINE_H_