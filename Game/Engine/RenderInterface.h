/*********************************************************************************

  *FileName: RenderInterface.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/02

**********************************************************************************/


#ifndef _RENDERINTERFACE_H_
#define _RENDERINTERFACE_H_

#include "defines.h"
#include "material.h"
#include "light.h"
#include "GUI.h"
#include "vertex.h"

class CRenderInterface
{
   public:
      CRenderInterface() : m_screenWidth(0),m_screenHeight(0), m_fullscreen(0), m_mainHandle(0), m_near(0), m_far(0) { }
      virtual ~CRenderInterface() {}
      
      virtual bool Initialize(int w, int h,WinHWND mainWin, bool fullScreen) = 0;
      virtual bool SetupImGui(WinHWND mainWin) = 0;
      virtual void StartImGuiFrame() = 0;
      virtual void OneTimeInit() = 0;
      virtual void Shutdown() = 0;
      
      virtual void SetClearCol(float r, float g, float b, float a) = 0;
     
      virtual void StartRender(bool bColor, bool bDepth,bool bStencil) = 0;
      virtual void ClearBuffers(bool bColor, bool bDepth,bool bStencil) = 0;
      virtual void EndRendering() = 0;

      virtual void SetMaterial(stMaterial* mat) = 0;
      virtual void SetLight(stLight* light, size_t index) = 0;
      virtual void DisableLight(size_t index) = 0;

      virtual bool SetMultiSaple(int samples) = 0;
      virtual void SetD3D9RenderState(RenderState state) = 0;
      virtual void SetDepthTesting(RenderState state) = 0;
      virtual void SetTransparent(RenderState state, TransperentState src, TransperentState dst) = 0;

      virtual int AddTexture2D(const char* file, int* texId) = 0;
      virtual void SetTextureFilter(size_t index, TextureState filter, FilterType type) = 0;
      virtual void SetMultiTexture() = 0;
      virtual void ApplyTexture(size_t index, int texId) = 0;
      virtual void SaveScreenShot(const char* file) = 0;

      virtual void EnablePointSprites(float size, float min, float a, float b, float c) = 0;
      virtual void DisablePointSprites() = 0;

      virtual bool CreateText(const char* font, int weight, bool italic, int size, int& id) = 0;
      virtual bool DisplayText(int id, long x, long y, int r, int g, int b, const char* text, ...) = 0;
      virtual bool DisplayText(int id, long x,long y, unsigned long color, const char* text, ...) = 0;

      bool CreateGUI(int& id)
      {
          if (!m_guiList)
          {
              m_guiList = new CGUISystem[1];
              if (!m_guiList) 
                  return UGP_FAIL;
          }
          else
          {
              CGUISystem* temp;
              temp = new CGUISystem[m_totalGUIs + 1];

              memcpy(temp, m_guiList,
                  sizeof(CGUISystem) * m_totalGUIs);

              delete[] m_guiList;
              m_guiList = temp;
          }

          id = m_totalGUIs;
          m_totalGUIs++;

          return true;
      }
      virtual bool AddGUIBackdrop(int guiId, const char* fileName) = 0;
      virtual bool AddGUIStaticText(int guiId, int id, const char* text, int x, int y, unsigned long color, int fontID) = 0;
      virtual bool AddGUIButton(int guiId, int id, int x, int y, const char* up, const char* over, const char* down) = 0;
      virtual void ProcessGUI(int guiID, bool LMBDown, int mouseX, int mouseY, void(*funcPtr)(int id, size_t state)) = 0;

      virtual void CalculateProjMatrix(float fov, float n,float f) = 0;
      virtual void CalculateOrthoMatrix(float n, float f) = 0;
      
      virtual int CreateStaticBuffer(VertexType, PrimType,
          size_t totalVerts, size_t totalIndices,
          size_t stride, void **data, size_t *indices,
          int*staticId) = 0;

      //virtual int Updata() = 0;
      virtual int Render(int staticId) = 0;

   protected:
      int m_screenWidth;
      int m_screenHeight;
      bool m_fullscreen;

      WinHWND m_mainHandle;
      
      float m_near;
      float m_far;

      int m_totalFonts;

      CGUISystem* m_guiList;
      int m_totalGUIs;
};

#endif