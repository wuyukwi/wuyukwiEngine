#pragma once
#include <iostream>
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Vector.h"
#include "structs.h"
#include "D3D9Renderer.h"

class Node
{
protected:
    std::vector<Node*> m_children{};        ///< array of children nodes
    //Node* _parent;                  ///< weak reference to parent node
    Vector2f m_xySize = 0;
    Vector2f m_pos = 0;
    Vector2f m_texPos = 0;
    bool m_isDelete = false;

public:
    Node() = default;
    virtual ~Node();

    virtual void AddChild(Node* sub_node)
    {
        this->m_children.push_back(sub_node);
    }

    virtual void SetSize(float x, float y);
    virtual void SetSize(const Vector2f& vec);

    virtual void SetPos(float x, float y);
    virtual void SetPos(const Vector2f& vec);
    virtual void SetPosX(float x);
    virtual void SetPosY(float y);
    virtual float GetPosX();
    virtual float GetPosY();

    virtual bool GetIsDelete();

    virtual void Start();

    virtual void Updata(float delta);

    virtual void Render();
};

class Sprite :public Node
{
protected:
    CRenderInterface* m_renderer;
    NodeVertex2D m_vertexBuffer[4];
    std::string m_texName;
    int32_t m_texId = -1;
    int32_t m_staticId = -1;
    int32_t m_key;
public:

    Sprite() = delete;
    ~Sprite() override;
    Sprite(int32_t key, const char* tex_name, CRenderInterface* renderer) : m_renderer(renderer), m_texName(tex_name),
        m_key(key)
    {
    }

    Sprite(int32_t key, std::string tex_name, CRenderInterface* renderer) : m_renderer(renderer),
        m_texName(std::move(tex_name)), m_key(key)
    {
    }

    Sprite(int32_t key, const std::string&& tex_name, CRenderInterface* renderer) : m_renderer(renderer),
        m_texName(tex_name), m_key(key)
    {
    }

    virtual void Start() override;
    virtual void Updata(float delta) override;
    virtual void Render() override;

    virtual int32_t Getkey();
};
