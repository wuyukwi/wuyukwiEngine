/*********************************************************************************

  *FileName: game.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/19

**********************************************************************************/

#ifndef _GAME_H_
#define _GAME_H_

#include "Engine/Camera.h"
#include "Engine/Matrix.h"
#include "Engine/Vector.h"
#include "stGameObject.h"
#include "Engine/Scene.h"

//*****************************************
// グローバル関数宣言
//*****************************************
bool GameInitialize();
void GameLoop();
void GameShutdown();
void GameReleaseAll();
void GameProcessInput();
void fpsRender();

#define MISSION_COMPLETE   1
#define MISSION_FAIL       2
#define MISSION_ONGOING    3

// Holds entire game world.
struct stGameWorld
{
    stGameWorld()
    {
        m_levelID = m_skyBoxID = -1;
        m_models = 0; m_totalModels = 0;
        m_artifacts = 0; m_totalArtifacts = 0;
        m_characters = 0; m_totalCharacters = 0;
        m_characterSpot = 0;
        m_artifactsCollected = 0;
        m_totalTime = m_startTime = m_currentTime = 0;
        m_spottedCount = m_numArtifactsCollected = 0;
        m_levelStartTime = 0;
        m_missionStatus = MISSION_ONGOING;

        g_totalEnemiesKilled = 0;
    }

    void Shutdown()
    {
        m_levelID = -1;
        m_skyBoxID = -1;

        if (m_models) delete[] m_models;
        m_models = 0;
        m_totalModels = 0;

        if (m_characters) delete[] m_characters;
        m_characters = 0;

        if (m_characterSpot) delete[] m_characterSpot;
        m_characterSpot = 0;
        m_totalArtifacts = 0;

        if (m_artifacts) delete[] m_artifacts;
        m_artifacts = 0;

        if (m_artifactsCollected) delete[] m_artifactsCollected;
        m_artifactsCollected = 0;
        m_totalArtifacts = 0;

        m_totalTime = 0;
        m_startTime = 0;
        m_currentTime = 0;
        m_levelStartTime = 0;
        m_spottedCount = 0;
        m_numArtifactsCollected = 0;
        m_missionStatus = MISSION_ONGOING;

        g_totalEnemiesKilled = 0;
    }

    // Level id and skydome.
    int m_levelID;
    int m_skyBoxID;

    // Number of models and model list.
    stGameObject* m_models;
    int m_totalModels;

    // Number of models and model list.
    stGameObject* m_characters;
    int* m_characterSpot;
    int m_totalCharacters;

    // Number of artifacts and list.
    stGameObject* m_artifacts;
    int* m_artifactsCollected;
    int m_totalArtifacts;

    // Game Statistics.
    float m_totalTime;
    float m_startTime;
    float m_currentTime;
    float m_levelStartTime;
    int m_spottedCount;
    int m_numArtifactsCollected;

    // Mission status.
    int m_missionStatus;

    // Number of enemies killed.
    int g_totalEnemiesKilled;
};

// Game states.
enum class GAME_STATE { GS_MENU = 1, GS_LEVEL, GS_LEVEL_1_SWITCH };

// Mouse button state information.
extern bool g_LMBDown;
extern int g_mouseX;
extern int g_mouseY;

// Game level and states.
extern stGameWorld g_gameWorld;
extern GAME_STATE g_gameState;

// Camera and view ray.
extern CCamera g_camera;

extern char g_fpsStr[16];
extern float g_elapsed;
extern int g_timeCount;

extern int g_shotSound;

extern Scene* g_scene;

#endif _GAME_H_
