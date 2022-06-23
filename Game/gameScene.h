#include "game.h"

#pragma once
class gameScene :public Scene
{
public:
    ~gameScene() override = default;

    void Start() override;

    void Updata(float delta) override;

    void Render() override;

private:

    Sprite* m_sprite = nullptr;
};

