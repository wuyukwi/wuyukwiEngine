#include "SceneManager.h"

bool SceneManager::Initialize()
{
    return true;
}

void SceneManager::ShutDown()
{

}

void SceneManager::AddScene(Scene* scene)
{
    m_sceneList.push_back(scene);
}

void SceneManager::Tick(float elapsed)
{
    for (auto it = m_sceneList.begin(); it < m_sceneList.end();)
    {
        // エレメントがdeleteされた場合はmapから削除
        if (*it == nullptr)
        {
            //　erase関数は次のエレメントのイテレータを返す
            it = m_sceneList.erase(it);
        }
        else
        {
            (*it)->Updata(elapsed);
            (*it)->Render();
        }
    }
}
