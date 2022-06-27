#pragma once
#include "engine.h"
#include "WindowsApplication.h"

class gameEngine
{
public:
    gameEngine();
    ~gameEngine();

    bool InitializeEngine(const char* path_file);
    void ShutdownEngine();

    bool Run() const;

    Timer* GetTimer() const { return m_pTimer; }

private:
    bool LoadScript(const char* path_file);
    bool LoadSoundScript();

    CScript* m_pScript;
    Timer* m_pTimer;
    SceneManager* m_pSceneManager;
    WindowsApplication* m_WinApp;
    CRenderInterface* m_Renderer;
    CInputInterface* m_Input;
    CSoundInterface* m_Sound;
    GfxConfiguration m_Config;

    int32_t m_arialID;       //フォントid
};

extern gameEngine* g_pEngine;

