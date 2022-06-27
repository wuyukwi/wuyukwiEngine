#pragma once

#include "Node.h"
#include "map"
#include "memory"

class Scene
{
private:
    std::map<int32_t, Node*> m_sceneNodeMap;

public:
    virtual ~Scene() = default;

    virtual void AddChild(int32_t key, Node* node) { m_sceneNodeMap[key] = node; }

    virtual void Start()
    {
        for (auto it = m_sceneNodeMap.begin(); it != m_sceneNodeMap.end(); ++it)
        {
            it->second->Start();
        }
    }

    virtual void Updata(float delta)
    {
        for (auto it = m_sceneNodeMap.begin(); it != m_sceneNodeMap.end();)
        {
            // エレメントがdeleteされた場合はmapから削除
            if (it->second == nullptr)
            {
                //　erase関数は次のエレメントのイテレータを返す
                it = m_sceneNodeMap.erase(it);
            }
            else
            {
                it->second->Updata(delta);

                ++it;
            }
        }
    }

    virtual void Render()
    {
        for (auto it = m_sceneNodeMap.begin(); it != m_sceneNodeMap.end();)
        {
            // エレメントがdeleteされた場合はmapから削除
            if (it->second == nullptr)
            {
                //　erase関数は次のエレメントのイテレータを返す
                it = m_sceneNodeMap.erase(it);
            }
            else
            {
                it->second->Render();

                ++it;
            }
        }
    }
};
