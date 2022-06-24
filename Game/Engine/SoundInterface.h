/*********************************************************************************

  *FileName: SoundInterface.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/12

**********************************************************************************/

#ifndef _SOUND_INTERFACE_H_
#define _SOUND_INTERFACE_H_

class CSoundSystemInterface
{
public:
    CSoundSystemInterface() {}
    virtual ~CSoundSystemInterface() {}

    virtual bool Initialize(const char* filepath) = 0;
    virtual bool AddSound(const char* soundfile, int numRepeats, int* id) = 0;
    virtual bool SetupSoundParameters(int id, float dopplerFactor, float rolloffFactor, float minDist, float maxDist) = 0;

    virtual void Play(int id) = 0;
    virtual void UpdateSoundPosition(int id, float x, float y, float z) = 0;
    virtual void Stop(int id) = 0;
    virtual void Shutdown() = 0;
};

class CSoundInterface
{
public:
    CSoundInterface() {}
    virtual ~CSoundInterface() {}

    virtual bool Initialize(const char* filepath) = 0;
    virtual bool AddSound(const char* soundfile, int numRepeats, const char* command) = 0;
    virtual bool SetupSoundParameters(int id, float dopplerFactor, float rolloffFactor, float minDist, float maxDist) = 0;

    virtual bool Play(const char* command) = 0;
    virtual void Play(int32_t id) = 0;
    virtual void UpdateSoundPosition(int id, float x, float y, float z) = 0;
    virtual bool Stop(const char* command) = 0;
    virtual void Stop(int32_t id) = 0;
    virtual void Shutdown() = 0;
};


#endif _SOUND_INTERFACE_H_
