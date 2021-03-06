/*********************************************************************************

  *FileName: DirectInput.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/12

**********************************************************************************/


#ifndef _DIRECTINPUT_H_
#define _DIRECTINPUT_H_

#include<dinput.h>
#include"InputInterface.h"
#include"defines.h"


class CKeyboard : public CDeviceInterface
{
   public:
      CKeyboard(LPDIRECTINPUT8 input, HWND hwnd);
      ~CKeyboard() { Shutdown(); }

      bool UpdateDevice();

      bool ButtonUp(size_t key);
      bool ButtonDown(size_t key);

      POINT GetPosition(bool delta = false);
      POINT GetZPosition();

      void Shutdown();

      // Keyboard device.
      LPDIRECTINPUTDEVICE8 m_pDevice;
      char m_keys[KEYS_SIZE];
      char m_oldKeys[KEYS_SIZE];
};


class CMouse : public CDeviceInterface
{
   public:
      CMouse(LPDIRECTINPUT8 input, HWND hwnd, bool exclusive);
      ~CMouse() { Shutdown(); }

      bool UpdateDevice();

      bool ButtonUp(size_t key);
      bool ButtonDown(size_t key);

      POINT GetPosition(bool delta = false);
      POINT GetZPosition();

      void Shutdown();

      // Mouse device.
      LPDIRECTINPUTDEVICE8 m_pDevice;
      DIMOUSESTATE m_mouseState;
      DIMOUSESTATE m_oldMouseState;
      bool m_button[3];

      // マウスのの位置
      POINT m_position;

      // 親ウィンドウのハンドル。
      HWND m_hwnd;

      // 最大x（幅）およびy（高さ）制限
      bool m_restrict;
      int m_width;
      int m_height;
};


class CGameController : public CDeviceInterface
{
   public:
      CGameController(LPDIRECTINPUT8 input, HWND hwnd);
      ~CGameController() { Shutdown(); }

      bool UpdateDevice();

      // Used to create the game controllers.
      BOOL EnumDeviceCallBack(const DIDEVICEINSTANCE *inst, void* pData);

      bool ButtonUp(size_t key);
      bool ButtonDown(size_t key);

      POINT GetPosition(bool delta = false);
      POINT GetZPosition();

      void Shutdown();

      // Game controller device.
      LPDIRECTINPUTDEVICE8 m_pDevice;
      DIJOYSTATE2 m_gcState;
      DIJOYSTATE2 m_oldGCState;
      char m_name[256];
      unsigned long m_numButtons;

      // Left and right stick x and y positions.
      long m_xGCPos;
      long m_yGCPos;
      long m_xGCPos2;
      long m_yGCPos2;
      
      // Window handled (needed for game controllers).
      HWND m_hwnd;

      // Copy of input system.
      LPDIRECTINPUT8 m_inputSystem;
};


class CDirectInputSystem : public CInputInterface
{
   public:
      CDirectInputSystem(HWND hwnd, HINSTANCE hInst, bool exclusive);
      ~CDirectInputSystem();

      // Init devics, update devices states.
      bool Initialize();
      bool UpdateDevices();
      
      // Keyboard functions.
      bool KeyUp(size_t key);
      bool KeyDown(size_t key);

      // Mouse functions.
      bool MouseButtonUp(size_t button);
      bool MouseButtonDown(size_t button);

      // Get mouse position (x, y) and mouse wheel data (z).
      POINT GetMousePos(bool delta = false);
      long GetMouseWheelPos();

      // Game controller functions.
      bool ControllerButtonUp(size_t button);
      bool ControllerButtonDown(size_t button);
      
      // Get controller main (left) and right stick position.
      POINT GetLeftStickPos();
      POINT GetRightStickPos();

      void Shutdown();

   protected:
      LPDIRECTINPUT8 m_system;

      // Devices.
      CKeyboard *m_keyboard;
      CMouse *m_mouse;
      CGameController *m_gameController;
};

bool CreateDIInput(CInputInterface **pObj, HWND hwnd, HINSTANCE hInst, bool exclusive);

#endif 