/*********************************************************************************

  *FileName: DirectSound.h
            �R�E  �L�K�N
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/12

**********************************************************************************/

#ifndef _DIRECTSOUND_H_
#define _DIRECTSOUND_H_

#define INITGUID

#include<windows.h>
#include"SoundInterface.h"
#include"defines.h"
#include <wrl/client.h>

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>
#include <xapofx.h>
#pragma comment(lib,"xaudio2.lib")

//-----------------------------------------------------------------------------
// Struct to hold audio game state
//-----------------------------------------------------------------------------
struct AUDIO_STATE
{
    int Repeats;

    IXAudio2SourceVoice* pSourceVoice;	// �\�[�X�{�C�X
    BYTE* pDataAudio;					// �I�[�f�B�I�f�[�^
    DWORD SizeAudio;					// �I�[�f�B�I�f�[�^�T�C�Y
};


class CDMSoundObject : public CSoundInterface
{
   public:
      CDMSoundObject();
      ~CDMSoundObject() { Shutdown(); }

      bool Initialize(const char* filepath);
      bool AddSound(char* soundfile, int numRepeats, int id);
      bool SetupSoundParameters(int id, float dopplerFactor,float rolloffFactor,float minDist, float maxDist);

      void Play(int id);
      void UpdateSoundPosition(int id, float x, float y, float z);
      void Stop(int id);
      void Shutdown();

   private:
    int IncreaseSounds();

    const char* m_filePath;
    bool m_comInit;
    IXAudio2* m_pXAudio2;							// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
    IXAudio2MasteringVoice* pMasterVoice;			// �}�X�^�[�{�C�X
    AUDIO_STATE* m_audioState;
    int m_totalSounds;

};


class CDirectMusicSystem : public CSoundSystemInterface
{
   public:
      CDirectMusicSystem();
      ~CDirectMusicSystem() { Shutdown(); }

      bool Initialize(const char* filepath);
      bool AddSound(const char *soundfile, int numRepeats, int *id);
      bool SetupSoundParameters(int id,float dopplerFactor,float rolloffFactor,float minDist, float maxDist);

      void Play(int id);
      void UpdateSoundPosition(int id, float x,float y, float z);
      void Stop(int id);
      void Shutdown();
      

   private:
      
      CDMSoundObject *m_soundList;
};

bool CreateDMSound(CSoundInterface**pObj);

#endif _DIRECTSOUND_H_