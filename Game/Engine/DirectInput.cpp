/*********************************************************************************

  *FileName: DirectInput.cpp
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/12

**********************************************************************************/


#include"DirectInput.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define MOUSE_BUFF_SIZE 16


CGameController *gThis = NULL;


bool CreateDIInput(CInputInterface **pObj, HWND hwnd, HINSTANCE hInst, bool exclusive)
{
   if(!*pObj) *pObj = new CDirectInputSystem(hwnd, hInst, exclusive);
   else return false;
   
   return true;
}


BOOL gJSEnumDeviceCallBack(const DIDEVICEINSTANCE *inst, void* pData)
{
   return gThis->EnumDeviceCallBack(inst, pData);
}


CKeyboard::CKeyboard(LPDIRECTINPUT8 input, HWND hwnd)
{
    // キーボードデバイスを作成、準備、および取得。
   if(input->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL) == DI_OK)
      {
         if(m_pDevice->SetDataFormat(&c_dfDIKeyboard) == DI_OK)
            {
               if(m_pDevice->SetCooperativeLevel(hwnd,
                  DISCL_FOREGROUND |DISCL_NONEXCLUSIVE) == DI_OK)
                  {
                     m_pDevice->Acquire();
                  }
            }
   }

   // 初期化処理
   memset(m_keys, 0, sizeof(m_keys));
}

bool CKeyboard::ButtonUp(size_t key)
{
   // 変数のキーが放した瞬間だけ、tureを返します。
   return (!(m_keys[key] & 0x80) && m_keys[key] != m_oldKeys[key]);
}


bool CKeyboard::ButtonDown(size_t key)
{
   // 変数のキーが押されていない場合は、falseを返します。
    if (m_keys[key] & 0x80)
        return true;
    else
        return false;
}


POINT CKeyboard::GetPosition(bool delta)
{
   // キーボードは位置状態なし
   POINT p = { 0, 0 };
   return p;
}


POINT CKeyboard::GetZPosition()
{
   // キーボードは位置状態なし
   POINT p = { 0, 0 };
   return p;
}


bool CKeyboard::UpdateDevice()
{
   if(m_pDevice)
      {
         // 入力比較のために古い状態を保存します。
         memcpy(m_oldKeys, m_keys, sizeof(m_keys));

         // 成功するか、不明なエラーが返されるまで、キーボードをポーリングします。
         if(FAILED(m_pDevice->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
            {
               if(FAILED(m_pDevice->Acquire())) 
                   return false;
               if(FAILED(m_pDevice->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
                  return false;
            }
       }

   return true;
}


void CKeyboard::Shutdown()
{
   if(m_pDevice)
      {
         m_pDevice->Unacquire();
         m_pDevice->Release();
         m_pDevice = NULL;
      }
}


CMouse::CMouse(LPDIRECTINPUT8 input, HWND hwnd, bool exclusive)
{
   // マウス初期化処理
   DWORD flags;
   m_hwnd = hwnd;

   if(input->CreateDevice(GUID_SysMouse, &m_pDevice, NULL) == DI_OK)
      {
         if(m_pDevice->SetDataFormat(&c_dfDIMouse) == DI_OK)
            {
               if(exclusive) flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
               else flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;

               if(m_pDevice->SetCooperativeLevel(hwnd, flags) == DI_OK)
                  {
                     m_pDevice->Acquire();
                  }
            }
      }
}


bool CMouse::ButtonUp(size_t button)
{
    // 変数のキーが放した瞬間だけ、tureを返します。
   return (!(m_mouseState.rgbButtons[button] & 0x80) &&m_mouseState.rgbButtons[button] != m_oldMouseState.rgbButtons[button]);
}


bool CMouse::ButtonDown(size_t button)
{
    // 変数のキーが押されていない場合は、falseを返します。
   if (m_mouseState.rgbButtons[button] & 0x80)
       return true;
   else
       return false;
}


bool CMouse::UpdateDevice()
{

   if(m_pDevice)
      {
         // 古いデータを保存
         memcpy(&m_oldMouseState, &m_mouseState, sizeof(m_mouseState));

         // デバイスの状態の取得中にエラーが発生した場合は、再取得する
         if(FAILED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
            {
               if(FAILED(m_pDevice->Acquire())) return false;
               if(FAILED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
                  return false;
            }

         // マウスの相対位置を取得します。
         GetCursorPos(&m_position);
         ScreenToClient(m_hwnd, &m_position);
      }

   return true;
}

POINT CMouse::GetPosition(bool delta)
{
    if (!delta)
        return m_position;
    else
    {
        POINT p = { m_mouseState.lX,m_mouseState.lY };

        return p;
    }
}


POINT CMouse::GetZPosition()
{
   POINT p = { m_mouseState.lZ, m_mouseState.lZ };
   return p;
}


void CMouse::Shutdown()
{
   if(m_pDevice)
      {
         m_pDevice->Unacquire();
         m_pDevice->Release();
         m_pDevice = NULL;
      }
}


CGameController::CGameController(LPDIRECTINPUT8 input, HWND hwnd)
{
   // Save copies.
   gThis = this;
   m_hwnd = hwnd;
   m_inputSystem = input;

   // Initialize the game controller.
   DIPROPRANGE range;
   DIDEVCAPS caps;

   m_inputSystem->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)gJSEnumDeviceCallBack,
                              NULL, DIEDFL_ATTACHEDONLY);

   if(m_pDevice)
      {
         range.diph.dwSize = sizeof(DIPROPRANGE);
         range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
         range.diph.dwHow = DIPH_BYOFFSET;
         range.lMin = -1000;
         range.lMax = 1000;
         range.diph.dwObj = DIJOFS_X;
         m_pDevice->SetProperty(DIPROP_RANGE, &range.diph);
         range.diph.dwObj = DIJOFS_Y;
         m_pDevice->SetProperty(DIPROP_RANGE, &range.diph);

         if(SUCCEEDED(m_pDevice->GetCapabilities(&caps))) m_numButtons = caps.dwButtons;
         else m_numButtons = 4;
      }
}


BOOL CGameController::EnumDeviceCallBack(const DIDEVICEINSTANCE *inst, void* pData)
{
   // Set to the first device found.
   if(SUCCEEDED(m_inputSystem->CreateDevice(inst->guidInstance, &m_pDevice, NULL)))
      {
         if(SUCCEEDED(m_pDevice->SetDataFormat(&c_dfDIJoystick2)))
            if(SUCCEEDED(m_pDevice->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
               if(SUCCEEDED(m_pDevice->Acquire()))
                  {
                     strcpy(m_name, (char*)inst->tszProductName);
                     return DIENUM_STOP;
                  }
      }

   // Return continue to try to init other connected devices.
   return DIENUM_CONTINUE;
}


bool CGameController::ButtonUp(size_t button)
{
   if(button < 0 || button >= m_numButtons) return false;
   return (!(m_gcState.rgbButtons[button] & 0x80) &&
           m_gcState.rgbButtons[button] != m_oldGCState.rgbButtons[button]);
}


bool CGameController::ButtonDown(size_t button)
{

   if(button < 0 || button >= m_numButtons) 
       return false;

   if (m_gcState.rgbButtons[button] & 0x80)
       return true;
   else
       return true;
}


bool CGameController::UpdateDevice()
{
   if(m_pDevice)
      {
         m_pDevice->Poll();

         // Save old state for input comparing.
         memcpy(&m_oldGCState, &m_gcState, sizeof(m_gcState));

         // If error getting device state, re-aquire.
         if(FAILED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &m_gcState)))
            {
               if(FAILED(m_pDevice->Acquire())) return false;
               if(FAILED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &m_gcState)))
                  return false;
            }

         m_xGCPos = m_gcState.lX;
         m_yGCPos = m_gcState.lY;

         m_xGCPos2 = m_gcState.lZ;
         m_yGCPos2 = m_gcState.lRz;
      }

   return true;
}


POINT CGameController::GetPosition(bool delta)
{
   POINT pos;

   pos.x = m_xGCPos;
   pos.y = m_yGCPos;
   return pos;
}


POINT CGameController::GetZPosition()
{
   POINT pos;

   pos.x = m_xGCPos2;
   pos.y = m_yGCPos2;
   return pos;
}


void CGameController::Shutdown()
{
   if(m_pDevice)
      {
         m_pDevice->Unacquire();
         m_pDevice->Release();
         m_pDevice = NULL;
      }
}


CDirectInputSystem::CDirectInputSystem(HWND hwnd, HINSTANCE hInst, bool exclusive)
{
   // Initialize objects...
   m_keyboard = NULL;
   m_mouse = NULL;
   m_gameController = NULL;

   // Create input system.
   if(DirectInput8Create(hInst, DIRECTINPUT_VERSION,
      IID_IDirectInput8, (void **)&m_system, NULL) == DI_OK)
   {
      m_keyboard = new CKeyboard(m_system, hwnd);
      m_mouse = new CMouse(m_system, hwnd, exclusive);
      m_gameController = new CGameController(m_system, hwnd);
   }
}


CDirectInputSystem::~CDirectInputSystem()
{
   // Shut everything down.
   Shutdown();
}


bool CDirectInputSystem::Initialize()
{
   // Everything took place in the constructor.
   // Force keyboard and mouse before considering the
   // system initialized.
   return (m_keyboard && m_mouse);
}


bool CDirectInputSystem::UpdateDevices()
{
   int hr;

   // Get the device state.
   if(m_mouse) hr = m_mouse->UpdateDevice();
   if(m_keyboard) hr = m_keyboard->UpdateDevice();
   if(m_gameController) hr = m_gameController->UpdateDevice();

   return true;
}


bool CDirectInputSystem::KeyUp(size_t key)
{
   if(!m_keyboard) return false;
   return m_keyboard->ButtonUp(key);
}


bool CDirectInputSystem::KeyDown(size_t key)
{
   if(!m_keyboard) return false;
   return m_keyboard->ButtonDown(key);
}


bool CDirectInputSystem::MouseButtonUp(size_t button)
{
   if(!m_mouse) return false;
   return m_mouse->ButtonUp(button);
}


bool CDirectInputSystem::MouseButtonDown(size_t button)
{
   if(!m_mouse) return false;
   return m_mouse->ButtonDown(button);
}


bool CDirectInputSystem::ControllerButtonUp(size_t button)
{
   if(!m_gameController) return false;
   return m_gameController->ButtonUp(button);
}


bool CDirectInputSystem::ControllerButtonDown(size_t button)
{
   if(!m_gameController) return false;
   return m_gameController->ButtonDown(button);
}


POINT CDirectInputSystem::GetMousePos(bool delta)
{
   POINT null = {0, 0};
   if(!m_mouse) return null;

   return m_mouse->GetPosition(delta);
}


long CDirectInputSystem::GetMouseWheelPos()
{
   if(!m_mouse) return 0;

   POINT wheel =  m_mouse->GetZPosition();
   return wheel.y;
}


POINT CDirectInputSystem::GetLeftStickPos()
{
   POINT null = {0, 0};
   if(!m_gameController) return null;

   return m_gameController->GetPosition();
}


POINT CDirectInputSystem::GetRightStickPos()
{
   POINT null = {0, 0};
   if(!m_gameController) return null;

   return m_gameController->GetZPosition();
}


void CDirectInputSystem::Shutdown()
{
   // Delete each object...
   if(m_keyboard)
      {
         m_keyboard->Shutdown();
         delete m_keyboard;
         m_keyboard = NULL;
      }

   if(m_mouse)
      {
         m_mouse->Shutdown();
         delete m_mouse;
         m_mouse = NULL;
      }

   if(m_gameController)
      {
         m_gameController->Shutdown();
         delete m_gameController;
         m_gameController = NULL;
      }

   if(m_system)
      {
         m_system->Release();
         m_system = NULL;
      }
}