#pragma once
#include "GfxConfiguration.h"

class ApplicationInterface
{
public:
    virtual ~ApplicationInterface() = default;

private:
    virtual bool Initialize(GfxConfiguration* cfg, bool enable_imgui = true) = 0;
    virtual void Shutdown() = 0;

    virtual void Tick() = 0;

};

