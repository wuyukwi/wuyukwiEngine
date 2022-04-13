/*********************************************************************************

  *FileName: D3D9Renderer.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/02

**********************************************************************************/

#ifndef _D3D_RENDERER_H_
#define _D3D_RENDERER_H_

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "structs.h"
#include "RenderInterface.h"
#include "backends\imgui_impl_dx9.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

struct stD3D9StaticBuffer
{
   stD3D9StaticBuffer() : vbPtr(0), ibPtr(0), numVerts(0),
                         numIndices(0), stride(0), fvf(0),
                         primType(PrimType::NULL_TYPE) {}

   LPDIRECT3DVERTEXBUFFER9 vbPtr;
   LPDIRECT3DINDEXBUFFER9 ibPtr;
   size_t numVerts;
   size_t numIndices;
   size_t stride;
   size_t fvf;
   PrimType primType;
};

struct stD3D9Texture
{
    stD3D9Texture() : fileName(0), image(0), width(0), height(0), enable(0) {}

    char* fileName;
    float width, height;
    bool enable;
    LPDIRECT3DTEXTURE9 image;
};

class CD3D9Renderer : public CRenderInterface
{
   public:
      CD3D9Renderer();
      ~CD3D9Renderer();
      
      bool Initialize(int w, int h, WinHWND mainWin, bool fullScreen, int multisample) override;

      bool SetupImGui(WinHWND mainWin) override;
      void StartImGuiFrame() override;

      void Shutdown() override;
      
      void SetClearCol(float r, float g, float b, float a) override;
      void StartRender(bool bColor, bool bDepth, bool bStencil) override;
      void ClearBuffers(bool bColor, bool bDepth, bool bStencil) override;
      void EndRendering();

      void SetMaterial(stMaterial* mat) override;
      void SetLight(stLight* light, size_t index) override;
      void DisableLight(size_t index) override;

      bool SetMultiSaple(int samples)  override;
      void EnableMultiSaple(bool samples) override;

      void SetD3D9RenderState(RenderState state) override;
      void SetDepthTesting(RenderState state) override;
      void SetTransparent(RenderState state, TransperentState src, TransperentState dst) override;

      int AddTexture2D(const char* file, int* texId) override;
      void SetTextureFilter(size_t index, TextureState filter, FilterType type) override;
      void SetMultiTexture() override;
      void SetDetailMapping() override;
      void ApplyTexture(size_t index, int texId) override;
      void SaveScreenShot(const char* file) override;

      virtual void EnablePointSprites(float size, float min, float a, float b, float c) override;
      virtual void DisablePointSprites() override;

      bool CreateText(const char* font, int weight, bool italic, int size, int& id) override;
      bool DisplayText(int id, long x, long y, int r, int g, int b, const char* text, ...) override;
      bool DisplayText(int id, long x, long y, unsigned long color, const char* text, ...) override;

      bool AddGUIBackdrop(int guiId, const char* fileName) override;
      bool AddGUIStaticText(int guiId, int id, const char* text, int x, int y, unsigned long color, int fontID) override;
      bool AddGUIButton(int guiId, int id, int x, int y, const char* up, const char* over, const char* down) override;
      void ProcessGUI(int guiID, bool LMBDown, int mouseX, int mouseY, void(*funcPtr)(int id, size_t state)) override;

      void CalculateProjMatrix(float fov, float n, float f) override;
      void CalculateOrthoMatrix(float n, float f) override;

      void EnableFog(float start, float end, FOG_TYPE type, unsigned long color, bool rangeFog) override;
      void DisableFog() override;

      // スタティックバファ作成関数
      int CreateStaticBuffer(VertexType, PrimType,
          size_t totalVerts, size_t totalIndices,
          size_t stride, void **data, size_t*indices,
          int*staticId) override;

      int Render(int staticId) override;

   private:
      void OneTimeInit();

   
   private:
      D3DXCOLOR m_Color;
      LPDIRECT3D9 m_Direct3D;
      LPDIRECT3DDEVICE9 m_Device;
      D3DDISPLAYMODE m_d3ddm;
      D3DPRESENT_PARAMETERS m_d3dpp;
      D3DCAPS9 m_caps;

      bool m_renderingScene;
      
      stD3D9StaticBuffer *m_staticBufferList;
      int m_numStaticBuffers;
      int m_activeStaticBuffer;

      stD3D9Texture* m_textureList;
      int m_numTextures;

      LPD3DXFONT* m_fonts;
};

bool CreateD3DRenderer(CRenderInterface **pObj);

#endif