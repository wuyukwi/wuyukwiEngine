/*********************************************************************************

  *FileName: InputInterface.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/12

**********************************************************************************/


#ifndef _INPUT_INTERFACE_H_
#define _INPUT_INTERFACE_H_


class CInputInterface
{
   public:
      CInputInterface(){}
      virtual ~CInputInterface(){}

      virtual bool Initialize() = 0;
      virtual bool UpdateDevices() = 0;

      virtual bool KeyUp(size_t key) = 0;
      virtual bool KeyDown(size_t key) = 0;

      virtual bool MouseButtonUp(size_t button) = 0;
      virtual bool MouseButtonDown(size_t button) = 0;
      virtual POINT GetMousePos() = 0;
      virtual long GetMouseWheelPos() = 0;

      virtual bool ControllerButtonUp(size_t button) = 0;
      virtual bool ControllerButtonDown(size_t button) = 0;
      virtual POINT GetLeftStickPos() = 0;
      virtual POINT GetRightStickPos() = 0;

      virtual void Shutdown() = 0;
};


class CDeviceInterface
{
   public:
      CDeviceInterface() {}
      virtual ~CDeviceInterface(){}

      virtual bool UpdateDevice() = 0;

      virtual bool ButtonUp(size_t key) = 0;
      virtual bool ButtonDown(size_t key) = 0;

      virtual POINT GetPosition() = 0;
      virtual POINT GetZPosition() = 0;

      virtual void Shutdown() = 0;
};

#endif