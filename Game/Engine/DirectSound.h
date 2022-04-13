/*********************************************************************************

  *FileName: DirectSound.h
            コウ  キガク
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

    IXAudio2SourceVoice* pSourceVoice;	// ソースボイス
    BYTE* pDataAudio;					// オーディオデータ
    DWORD SizeAudio;					// オーディオデータサイズ
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
    IXAudio2* m_pXAudio2;							// XAudio2オブジェクトへのインターフェイス
    IXAudio2MasteringVoice* pMasterVoice;			// マスターボイス
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