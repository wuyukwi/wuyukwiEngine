#pragma once
#include "Scene.h"

class SceneManager
{
public:

    virtual bool Initialize();
    virtual void ShutDown();

    virtual void AddScene(Scene* scene);
    virtual void Tick(float elapsed);

private:
    std::vector<Scene*> m_sceneList;
};

