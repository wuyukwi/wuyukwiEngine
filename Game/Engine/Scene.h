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
        for (auto itr = m_sceneNodeMap.begin(); itr != m_sceneNodeMap.end(); ++itr)
        {
            itr->second->Start();
        }
    }

    virtual void Updata(float delta)
    {
        for (auto itr = m_sceneNodeMap.begin(); itr != m_sceneNodeMap.end(); ++itr)
        {
            itr->second->Updata(delta);

            if (itr->second->GetIsDelete())
            {
                m_sceneNodeMap.erase(itr);
            }

        }
    }

    virtual void Render()
    {
        for (auto itr = m_sceneNodeMap.begin(); itr != m_sceneNodeMap.end(); ++itr)
        {
            itr->second->Render();

            if (itr->second->GetIsDelete())
            {
                m_sceneNodeMap.erase(itr);
            }
        }
    }
};
