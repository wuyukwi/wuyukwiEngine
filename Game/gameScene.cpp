#include "gameScene.h"
#include "gameEngine.h"

void gameScene::Start()
{
    m_sprite = new Sprite(0, "menu/level1Down.png", g_Render);
    m_sprite->SetPos(500.f, 500.f);
    m_sprite->SetSize(100.0f, 100.0f);
    g_scene->AddChild(m_sprite->Getkey(), m_sprite);

    Scene::Start();
}

void gameScene::Updata(float delta)
{
    if (g_Input->KeyDown(DIK_UP))
    {
        m_sprite->SetPosY(m_sprite->GetPosY() - 100.0f * delta);
    }
    else if (g_Input->KeyDown(DIK_DOWN))
    {
        m_sprite->SetPosY(m_sprite->GetPosY() + 100.0f * delta);
    }

    if (g_Input->KeyDown(DIK_LEFT))
    {
        m_sprite->SetPosX(m_sprite->GetPosX() - 100.0f * delta);
    }
    else if (g_Input->KeyDown(DIK_RIGHT))
    {
        m_sprite->SetPosX(m_sprite->GetPosX() + 100.0f * delta);
    }
    Scene::Updata(delta);
}

void gameScene::Render()
{
    Scene::Render();
}
