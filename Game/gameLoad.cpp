/*********************************************************************************

  *FileName: gameEngine.cpp
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/19

**********************************************************************************/

#include "gameLoad.h"
#include "main.h"
#include "Engine/Script.h"
#include "Engine/LogSystem.h"

bool GameWorldLoad(const char* file)
{
    if (!g_Render || !file) return false;

    CLogSystem gameLog;
    CScript script;

    char command[MAX_COMMAND_SIZE];
    char param[MAX_PARAM_SIZE];
    int totalModels = 0;
    int currentModelNum = 0;
    int totalArtifacts = 0;
    int currentArtifactNum = 0;
    int totalCharacters = 0;
    int currentCharacterNum = 0;

    if (!script.LoadScriptFile(file))
        return false;

    for (int i = 0; i < script.GetTotalLines(); i++)
    {
        script.ParseCommand(command);

        if (_stricmp(command, "LoadSkyBoxAsX") == 0) {}
        else if (_stricmp(command, "LoadTerrainAsX") == 0) {}
        else if (_stricmp(command, "LoadStaticModelAsX") == 0)
        {
            totalModels++;
        }
        else if (_stricmp(command, "LoadAnimatedModelAsX") == 0)
        {
            totalModels++;
        }
        else if (_stricmp(command, "LoadArtifactModelAsX") == 0)
        {
            totalArtifacts++;
        }
        else if (_stricmp(command, "LoadCharacterModelAsX") == 0)
        {
            totalCharacters++;
        }
        else if (_stricmp(command, "SetTimer") == 0) {}
        else if (_stricmp(command, "#") == 0) {}
        else if (_stricmp(command, "\0") == 0) {}
        else if (_stricmp(command, "SetCamera") == 0) {}
        else
        {
            gameLog.AppendLog("ErrorLog.txt","Error loading level on %d.\n",
                script.GetCurrentLineNum() + 1);

            script.Shutdown();
            return false;
        }

        script.MoveToNextLine();
    }

    script.MoveToStart();

    if (g_gameWorld.m_models)
        delete[] g_gameWorld.m_models;

    g_gameWorld.m_models = new stGameObject[totalModels];
    g_gameWorld.m_artifacts = new stGameObject[totalArtifacts];
    g_gameWorld.m_artifactsCollected = new int[totalArtifacts];
    ZeroMemory(g_gameWorld.m_artifactsCollected, sizeof(int) * totalArtifacts);

    g_gameWorld.m_characters = new stGameObject[totalCharacters];
    g_gameWorld.m_characterSpot = new int[totalCharacters];
    ZeroMemory(g_gameWorld.m_characterSpot, sizeof(int) * totalCharacters);

    for (int i = 0; i < script.GetTotalLines(); i++)
    {
        script.ParseCommand(command);

        if (_stricmp(command, "LoadSkyBoxAsX") == 0)
        {
            script.ParseStringParam(param);

            if (g_gameWorld.m_skyBoxID == -1)
            {
                if (!g_Render->LoadXModel(param, &g_gameWorld.m_skyBoxID))
                    return false;
            }
            else
            {
                if (!g_Render->LoadXModel(param, g_gameWorld.m_skyBoxID))
                    return false;
            }
        }
        else if (_stricmp(command, "LoadTerrainAsX") == 0)
        {
            script.ParseStringParam(param);

            if (g_gameWorld.m_levelID == -1)
            {
                if (!g_Render->LoadXModel(param, &g_gameWorld.m_levelID))
                    return false;
            }
            else
            {
                if (!g_Render->LoadXModel(param, g_gameWorld.m_levelID))
                    return false;
            }
        }
        else if (_stricmp(command, "LoadStaticModelAsX") == 0)
        {
            stGameObject* obj = NULL;
            obj = &g_gameWorld.m_models[currentModelNum++];

            if (obj)
            {
                script.ParseStringParam(param);

                if (!g_Render->LoadXModel(param, &obj->m_id))
                    return false;

                obj->m_type = X_MESH;
                obj->m_x = script.ParseFloatParam();
                obj->m_y = script.ParseFloatParam();
                obj->m_z = script.ParseFloatParam();
                obj->m_rotX = script.ParseFloatParam();
                obj->m_rotY = script.ParseFloatParam();
                obj->m_rotZ = script.ParseFloatParam();
            }
        }
        else if (_stricmp(command, "LoadAnimatedModelAsX") == 0)
        {
            stGameObject* obj = NULL;
            obj = &g_gameWorld.m_models[currentModelNum++];

            if (obj)
            {
                script.ParseStringParam(param);

                if (!g_Render->LoadXModel(param, &obj->m_id))
                    return false;

                obj->m_type = X_ANIMATED_MESH;
                obj->m_x = script.ParseFloatParam();
                obj->m_y = script.ParseFloatParam();
                obj->m_z = script.ParseFloatParam();
                obj->m_rotX = script.ParseFloatParam();
                obj->m_rotY = script.ParseFloatParam();
                obj->m_rotZ = script.ParseFloatParam();
            }
        }
        else if (_stricmp(command, "LoadArtifactModelAsX") == 0)
        {
            stGameObject* obj = NULL;
            obj = &g_gameWorld.m_artifacts[currentArtifactNum++];

            if (obj)
            {
                script.ParseStringParam(param);

                if (!g_Render->LoadXModel(param, &obj->m_id))
                    return false;

                obj->m_type = X_ANIMATED_MESH;
                obj->m_x = script.ParseFloatParam();
                obj->m_y = script.ParseFloatParam();
                obj->m_z = script.ParseFloatParam();
                obj->m_rotX = script.ParseFloatParam();
                obj->m_rotY = script.ParseFloatParam();
                obj->m_rotZ = script.ParseFloatParam();
            }
        }
        else if (_stricmp(command, "LoadCharacterModelAsX") == 0)
        {
            stGameObject* obj = NULL;
            obj = &g_gameWorld.m_characters[currentCharacterNum++];

            if (obj)
            {
                script.ParseStringParam(param);

                if (!g_Render->LoadXModel(param, &obj->m_id))
                    return false;

                obj->m_type = X_ANIMATED_MESH;
                obj->m_x = script.ParseFloatParam();
                obj->m_y = script.ParseFloatParam();
                obj->m_z = script.ParseFloatParam();
                obj->m_rotX = script.ParseFloatParam();
                obj->m_rotY = script.ParseFloatParam();
                obj->m_rotZ = script.ParseFloatParam();
            }
        }
        else if (_stricmp(command, "#") == 0) {}
        else if (_stricmp(command, "\0") == 0) {}
        else if (_stricmp(command, "SetCamera") == 0)
            {
           
            }

      
        script.MoveToNextLine();
    }

    g_gameWorld.m_totalModels = totalModels;
    g_gameWorld.m_totalArtifacts = totalArtifacts;
    g_gameWorld.m_totalCharacters = totalCharacters;

    for (int i = 0; i < g_gameWorld.m_totalModels; i++)
    {
        BuildMatrixIdentity(g_gameWorld.m_models[i].worldMat);
    }

    for (int i = 0; i < g_gameWorld.m_totalArtifacts; i++)
    {
        BuildMatrixIdentity(g_gameWorld.m_artifacts[i].worldMat);
    }

    for (int i = 0; i < g_gameWorld.m_totalCharacters; i++)
    {
        BuildMatrixIdentity(g_gameWorld.m_characters[i].worldMat);
    }

    g_gameWorld.m_levelStartTime = (float)g_timeCount;

    script.Shutdown();

    return true;
}


//bool TestArtifactCollision(int i)
//{
//    CBoundingSphere sphere;
//    int index = g_gameWorld.m_artifacts[i].m_id;
//
//    g_Render->GetXModelBoundingSphere(index,&sphere.m_center, &sphere.m_radius);
//
//    Matrix4x4f* worldMat = &g_gameWorld.m_artifacts[i].worldMat;
//    sphere.m_center = worldMat->VectorMatrixMultiply(sphere.m_center);
//
//    if (sphere.isPointInside(g_camera.m_pos))
//        return true;
//
//    return false;
//}


void GameWorldRender()
{
    if (!g_Render || g_gameState != GAME_STATE::GS_LEVEL) return;

    Matrix4x4f world;
    BuildMatrixIdentity(world);

    g_Render->SetViewMatrix(g_camera.GetCameraView());
    
    g_Render->SetWorldMatrix(&world);
    g_Render->RenderXModel(g_gameWorld.m_levelID);

    g_Render->SetWorldMatrix(&world);
    g_Render->RenderXModel(g_gameWorld.m_skyBoxID);

    g_Render->SetD3D9RenderState(RenderState::CULL_CCW);

    if (g_gameWorld.m_models)
    {
        for (int i = 0; i < g_gameWorld.m_totalModels; i++)
        {
            g_Render->SetWorldMatrix(&g_gameWorld.m_models[i].worldMat);

            if (g_gameWorld.m_models[i].m_type == X_ANIMATED_MESH)
                g_Render->UpdateXAnimation(g_gameWorld.m_models[i].m_id,g_elapsed,&g_gameWorld.m_models[i].worldMat);
            else
                g_Render->UpdateXAnimation(g_gameWorld.m_models[i].m_id, 0,&g_gameWorld.m_models[i].worldMat);

            g_Render->RenderXModel(g_gameWorld.m_models[i].m_id);
        }
    }


    if (g_gameWorld.m_artifacts)
    {
        for (int i = 0; i < g_gameWorld.m_totalArtifacts; i++)
        {
            if (!g_gameWorld.m_artifactsCollected[i])
            {
                g_Render->UpdateXAnimation(g_gameWorld.m_artifacts[i].m_id, g_elapsed, &g_gameWorld.m_artifacts[i].worldMat);
                g_Render->SetWorldMatrix(&g_gameWorld.m_artifacts[i].worldMat);
                g_Render->RenderXModel(g_gameWorld.m_artifacts[i].m_id);
            }
        }
    }

    if(g_gameWorld.m_characters)
    {
        for (int i = 0; i < g_gameWorld.m_totalCharacters; i++)
        {
            if (g_gameWorld.m_characters[i].m_visible)
            {
                g_Render->SetWorldMatrix(&g_gameWorld.m_characters[i].worldMat);

               
                g_Render->UpdateXAnimation(g_gameWorld.m_characters[i].m_id, g_elapsed,&g_gameWorld.m_characters[i].worldMat);
             

                g_Render->RenderXModel(g_gameWorld.m_characters[i].m_id);
            }
        }
    }

}