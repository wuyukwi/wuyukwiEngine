/*********************************************************************************

  *FileName: RenderInterface.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/02

**********************************************************************************/

#ifndef _RENDERINTERFACE_H_
#define _RENDERINTERFACE_H_

#include <vector>
#include "defines.h"
#include "structs.h"
#include "material.h"
#include "light.h"
#include "GUI.h"
#include "Matrix.h"
#include "Camera.h"

class CRenderInterface
{
public:
    CRenderInterface() : m_screenWidth(0), m_screenHeight(0), m_fullscreen(false), m_mainHandle(nullptr), m_near(0), m_far(0) { }
    virtual ~CRenderInterface() {}

    virtual bool Initialize(int w, int h, WinHWND mainWin, bool fullScreen, int multisample) = 0;
    virtual bool SetupImGui(WinHWND mainWin) = 0;
    virtual void StartImGuiFrame() = 0;
    virtual void OneTimeInit() = 0;
    virtual void Shutdown() = 0;

    virtual void SetClearCol(float r, float g, float b, float a) = 0;

    virtual void StartRender(bool bColor, bool bDepth, bool bStencil) = 0;
    virtual void ClearBuffers(bool bColor, bool bDepth, bool bStencil) = 0;
    virtual void EndRendering() = 0;

    virtual void SetMaterial(stMaterial* mat) = 0;
    virtual void SetLight(stLight* light, size_t index) = 0;
    virtual void DisableLight(size_t index) = 0;

    virtual bool SetMultiSaple(int samples) = 0;
    virtual void EnableMultiSaple(bool samples) = 0;

    virtual void SetD3D9RenderState(RenderState state) = 0;
    virtual void SetDepthTesting(RenderState state) = 0;
    virtual void SetTransparent(RenderState state, TransperentState src, TransperentState dst) = 0;

    virtual int AddTexture2D(const char* file, int& texId) = 0;
    virtual int AddTexture2D(std::string& file_name, int& texId) = 0;
    virtual void SetTextureFilter(size_t index, TextureState filter, FilterType type) = 0;
    virtual void SetMultiTexture() = 0;
    virtual void SetDetailMapping() = 0;
    virtual void ApplyTexture(size_t index, int texId) = 0;
    virtual void SaveScreenShot(const char* file) = 0;

    virtual void EnablePointSprites(float size, float min, float a, float b, float c) = 0;
    virtual void DisablePointSprites() = 0;

    virtual bool CreateText(const char* font, int weight, bool italic, int size, int& id) = 0;
    virtual bool DisplayText(int id, long x, long y, int r, int g, int b, const char* text, ...) = 0;
    virtual bool DisplayText(int id, long x, long y, unsigned long color, const char* text, ...) = 0;

    virtual void EnableFog(float start, float end, FOG_TYPE type, unsigned long color, bool rangeFog) = 0;
    virtual void DisableFog() = 0;

    bool CreateGUI(int& id)
    {
        CGUISystem temp;

        id = this->m_guiList.size();
        this->m_guiList.push_back(temp);

        return true;
    }

    void ReleaseAllGUIs()
    {
        for (size_t s = 0; s < m_guiList.size(); s++)
        {
            m_guiList[s].Shutdown();
        }

        m_guiList.shrink_to_fit();
    }

    virtual bool AddGUIBackdrop(int guiId, const char* fileName) = 0;
    virtual bool AddGUIStaticText(int guiId, int id, const char* text, int x, int y, unsigned long color, int fontID) = 0;
    virtual bool AddGUIButton(int guiId, int id, int x, int y, const char* up, const char* over, const char* down) = 0;
    virtual void ProcessGUI(int guiID, bool LMBDown, int mouseX, int mouseY, void(*funcPtr)(int id, size_t state)) = 0;

    virtual void CalculateProjMatrix(float fov, float n, float f) = 0;
    virtual void CalculateOrthoMatrix(float n, float f) = 0;

    virtual int CreateStaticBuffer(VertexType, PrimType,
        size_t totalVerts, size_t totalIndices,
        size_t stride, void** data, size_t* indices,
        int* staticId) = 0;

    virtual int LoadXModel(char* file, int* xModelId) = 0;
    virtual int LoadXModel(char* file, int xModelId) = 0;

    virtual void ReleaseAllStaticBuffers() = 0;
    virtual void ReleaseAllXModels() = 0;
    virtual int ReleaseStaticBuffer(int staticID) = 0;
    virtual int ReleaseXModel(int xModelId) = 0;

    virtual int RenderStaticBuffer(int32_t staticId) = 0;
    virtual int RenderXModel(int xModelId) = 0;

    virtual void GetXModelBoundingSphere(int xModelId, Vector3f* origin, float* radius) = 0;

    virtual void UpdateXAnimation(int xModelId, float time, Matrix4x4f* mat) = 0;

    virtual void SetViewMatrix(Matrix4x4f* mat) = 0;
    virtual void SetWorldMatrix(Matrix4x4f* mat) = 0;

    virtual void GetProjectMatrix(Matrix4x4f* mat) = 0;

protected:
    int m_screenWidth;
    int m_screenHeight;
    bool m_fullscreen;

    WinHWND m_mainHandle;

    float m_near;
    float m_far;

    std::vector<CGUISystem> m_guiList;
};

#endif