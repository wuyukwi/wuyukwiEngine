#include "gameEngine.h"

gameEngine::gameEngine() : m_WinApp(nullptr), m_Renderer(nullptr), m_Input(nullptr), m_Sound(nullptr), m_arialID(0)
{
}

gameEngine::~gameEngine()
= default;

bool gameEngine::InitializeEngine(const char* path_file)
{
    LoadScript(path_file);

    m_pSceneManager = new SceneManager();

    m_pTimer = new Timer();

    m_WinApp = new WindowsApplication{};

    if (m_WinApp->Initialize(&m_Config))
    {
        return false;
    }
    if (!CreateD3DRenderer(&m_Renderer))
    {
        return false;
    }

    if (!m_Renderer->Initialize(m_Config.screenWidth, m_Config.screenHeight, m_WinApp->GetMainWindow(), m_Config.fullscreen, m_Config.msaaSamples))
    {
        return false;
    }
    m_Renderer->SetClearCol(1.0f, 1.0f, 1.0f, 1.0f);

    if (!m_Renderer->CreateText("Arial", 0, true, 18, m_arialID))
    {
        return false;
    }

    if (!CreateDIInput(&m_Input, m_WinApp->GetMainWindow(), m_WinApp->GetHinstance(), false) || !m_Input->Initialize())
    {
        return false;
    }

    if (!CreateDMSound(&m_Sound) || !LoadSoundScript())
    {
        return false;
    }

    return true;
}

void gameEngine::ShutdownEngine()
{
    m_WinApp->Shutdown();

    // エンジン終了処理
    if (m_Renderer)
    {
        m_Renderer->Shutdown();
        delete m_Renderer;
        m_Renderer = nullptr;
    }

    // Input終了処理
    if (m_Input)
    {
        m_Input->Shutdown();
        delete m_Input;
        m_Input = nullptr;
    }

    if (m_Sound)
    {
        m_Sound->Shutdown();
        delete m_Sound;
        m_Sound = nullptr;
    }
}

bool gameEngine::Run() const
{
    while (m_WinApp->IsQuit())
    {
        m_pTimer->Tick();
        m_WinApp->Tick();
        m_pSceneManager->Tick(m_pTimer->GetElapsed());
    }

    return true;
}

bool gameEngine::LoadScript(const char* path_file)
{
    m_pScript = new CScript{};

    if (!m_pScript->LoadScriptFile(path_file))
        return false;
    // ループして、指定された各コマンドを実行します。
    for (int i = 0; i < m_pScript->GetTotalLines(); i++)
    {
        char command[MAX_COMMAND_SIZE];
        m_pScript->ParseCommand(command);

        if (_stricmp(command, "WINDOW_CLASS") == 0)
        {
            char temp[MAX_COMMAND_SIZE];
            m_pScript->ParseStringParam(temp);
            m_Config.winClassName = temp;
        }
        else if (_stricmp(command, "WINDOW_NAME") == 0)
        {
            char temp[MAX_COMMAND_SIZE];
            m_pScript->ParseStringParam(temp);
            m_Config.appName = temp;
        }
        else if (_stricmp(command, "WIN_WIDTH") == 0)
        {
            m_Config.screenWidth = m_pScript->ParseIntParam();
        }
        else if (_stricmp(command, "WIN_HEIGHT") == 0)
        {
            m_Config.screenHeight = m_pScript->ParseIntParam();
        }
        else if (_stricmp(command, "FULLSCREEN") == 0)
        {
            m_Config.fullscreen = m_pScript->ParseBoolParam();
        }
        else if (_stricmp(command, "MULTI_SAMPLE") == 0)
        {
            m_Config.msaaSamples = m_pScript->ParseIntParam();
        }
        else if (_stricmp(command, "SOUNDS_PATH") == 0)
        {
            char temp[MAX_COMMAND_SIZE];
            m_pScript->ParseStringParam(temp);
            m_Config.soundPath = temp;
        }
        else if (_stricmp(command, "SOUNDS_FILE") == 0)
        {
            char temp[MAX_COMMAND_SIZE];
            m_pScript->ParseStringParam(temp);
            m_Config.soundFile = temp;
        }
        else if (_stricmp(command, "#") == 0)
        {
            // 何にもしません
        }
        else
        {
            // 何にもしません
        }
        // スクリプトの次の行に移動します。
        m_pScript->MoveToNextLine();
    }

    // すべてのリソースを解放します。このスクリプトは、初期化中にのみ実行されるように設計されています。
    m_pScript->Shutdown();

    return true;
}

bool gameEngine::LoadSoundScript()
{
    m_Sound->Initialize(m_Config.soundPath);
    m_pScript->LoadScriptFile(m_Config.soundFile);

    int repeats = 0;
    int id = 0;

    // ループして、指定された各コマンドを実行します。
    for (int i = 0; i < m_pScript->GetTotalLines(); i++)
    {
        char fileName[MAX_COMMAND_SIZE];
        char command[MAX_COMMAND_SIZE];
        m_pScript->ParseCommand(command);

        if (_stricmp(command, "MENU_SOUND") == 0)
        {
            m_pScript->ParseStringParam(fileName);
            repeats = m_pScript->ParseIntParam();
            m_Sound->AddSound(fileName, repeats, command);
        }
        else if (_stricmp(command, "SHOT_SOUND") == 0)
        {
            m_pScript->ParseStringParam(fileName);
            repeats = m_pScript->ParseIntParam();
            m_Sound->AddSound(fileName, repeats, command);
        }

        m_pScript->MoveToNextLine();
    }

    // すべてのリソースを解放します。このスクリプトは、初期化中にのみ実行されるように設計されています。
    m_pScript->Shutdown();
    return true;
}
