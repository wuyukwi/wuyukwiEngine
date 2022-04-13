//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
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
	SOUND_LABEL_SE_SHOT,				// �e���ˉ�
	SOUND_LABEL_SE_HIT0,				// �q�b�g��0
	SOUND_LABEL_SE_HIT1,				// �q�b�g��1
	SOUND_LABEL_SE_EXPLOSION,			// ������
	SOUND_LABEL_SE_PAUSE,				// �|�[�Y��
	SOUND_LABEL_SE_SELECT,				// �Z���N�g��
	SOUND_LABEL_SE_CANCEL,				// �L�����Z����
	SOUND_LABEL_SE_DECIDE,				// ���艹
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
