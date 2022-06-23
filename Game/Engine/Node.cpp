﻿#include "Node.h"
#include "D3D9Renderer.h"

Node::~Node()
{
    delete[] m_vertexBuffer;
}

void Node::SetSize(float x, float y)
{
    m_xySize.x = x; m_xySize.y = y;
}

void Node::SetSize(const Vector2f& vec)
{
    m_xySize = vec;
}

void Node::SetPos(float x, float y)
{
    m_pos.x = x; m_pos.y = y;
}

void Node::SetPos(const Vector2f& vec)
{
    m_pos = vec;
}

void Node::SetPosX(float x)
{
    m_pos.x = x;
}

void Node::SetPosY(float y)
{
    m_pos.y = y;
}

float Node::GetPosX()
{
    return m_pos.x;
}

float Node::GetPosY()
{
    return m_pos.y;
}

bool Node::GetIsDelete()
{
    return m_isDelete;
}

void Node::Start()
{
    for (auto itr = m_children.begin(); itr < m_children.end(); ++itr)
    {
        (*itr)->Start();
    }
    m_vertexBuffer = new NodeVertex2D[4];
}

void Node::Updata(float delta)
{
    for (auto itr = m_children.begin(); itr < m_children.end(); ++itr)
    {
        (*itr)->Updata(delta);
        if ((*itr)->GetIsDelete())
        {
            m_children.erase(itr);
        }
    }

    if (m_vertexBuffer == nullptr)
    {
        m_vertexBuffer = new NodeVertex2D[4];
    }

    m_vertexBuffer[0] = NodeVertex2D(m_pos.x - m_xySize.x, m_pos.y - m_xySize.y, 0.0f, 0.0f);
    m_vertexBuffer[1] = NodeVertex2D(m_pos.x - m_xySize.x, m_pos.y + m_xySize.y, 0.0f, 1.0f);
    m_vertexBuffer[2] = NodeVertex2D(m_pos.x + m_xySize.x, m_pos.y - m_xySize.y, 1.0f, 0.0f);
    m_vertexBuffer[3] = NodeVertex2D(m_pos.x + m_xySize.x, m_pos.y + m_xySize.y, 1.0f, 1.0f);

}

void Node::Render()
{
    for (auto itr = m_children.begin(); itr < m_children.end(); ++itr)
    {
        (*itr)->Render();
        if ((*itr)->GetIsDelete())
        {
            m_children.erase(itr);
        }
    }
}

Sprite::~Sprite()
= default;

void Sprite::Start()
{
    Node::Start();

    m_renderer->AddTexture2D(m_texName, m_texId);

}

void Sprite::Updata(float delta)
{
    Node::Updata(delta);


    if (!m_renderer->CreateStaticBuffer(
        FVF_2D, PrimType::TRIANGLE_STRIP,
        4, NULL, sizeof(NodeVertex2D),
        reinterpret_cast<void**>(m_vertexBuffer), nullptr, &m_staticId))
    {
        assert("Sprite CreateStaticBuffer error", m_staticId > 0);
    }
}

void Sprite::Render()
{
    Node::Render();

    m_renderer->ApplyTexture(1, m_texId);
    m_renderer->RenderStaticBuffer(m_staticId);
}

int32_t Sprite::Getkey()
{
    return m_key;
}