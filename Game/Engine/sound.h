//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BATTLEBAN = 0,			// BGM0
	SOUND_LABEL_BATTLEMEETING000,		// BGM1
	SOUND_LABEL_BATTLEMEETING001,		// BGM2
	SOUND_LABEL_CHALLENGE,				// BGM3
	SOUND_LABEL_JAGJAG,					// BGM4
	SOUND_LABEL_LAST_MEETING,			// BGM5
	SOUND_LABEL_NO_NAME,				// BGM6
	SOUND_LABEL_RESULT_BGM,				// BGM7
	SOUND_LABEL_RISE,					// BGM8
	SOUND_LABEL_START_BEETS,			// BGM9
	SOUND_LABEL_VIRTUAL_MISSION,		// BGM10
	SOUND_LABEL_SE_SHOT,				// 弾発射音
	SOUND_LABEL_SE_HIT0,				// ヒット音0
	SOUND_LABEL_SE_HIT1,				// ヒット音1
	SOUND_LABEL_SE_EXPLOSION,			// 爆発音
	SOUND_LABEL_SE_PAUSE,				// ポーズ音
	SOUND_LABEL_SE_SELECT,				// セレクト音
	SOUND_LABEL_SE_CANCEL,				// キャンセル音
	SOUND_LABEL_SE_DECIDE,				// 決定音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
