/********************************************************************************* 

  *FileName: input.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  


#include "engine.h"

//-----------------------------------------------------------------------------
// Input クラスコンストラクター。
//-----------------------------------------------------------------------------
Input::Input(HWND window)
{
	// 親ウィンドウへのハンドルを保存します。
	m_window = window;

	// DirectInputインターフェースを作成します。
	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_di, NULL);

	// キーボードデバイスを作成、準備、および取得します。
	m_di->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	m_keyboard->SetCooperativeLevel(m_window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_keyboard->Acquire();

	// マウスデバイスを作成、準備、および取得します。
	m_di->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	m_mouse->SetDataFormat(&c_dfDIMouse);
	m_mouse->SetCooperativeLevel(m_window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_mouse->Acquire();

	// プレススタンプ初期化
	m_pressStamp = 0;
}

//-----------------------------------------------------------------------------
// Input クラス デストラクタ。
//-----------------------------------------------------------------------------
Input::~Input()
{
	SAFE_RELEASE(m_di);
	SAFE_RELEASE(m_keyboard);
	SAFE_RELEASE(m_mouse);
}

//-----------------------------------------------------------------------------
// キーボードとマウスデバイスの状態を更新します。
//-----------------------------------------------------------------------------
void Input::Update()
{
	static HRESULT result;

	// 成功するか、不明なエラーが返されるまで、キーボードをポーリングします。
	while (true)
	{
		m_keyboard->Poll();
		if (SUCCEEDED(result = m_keyboard->GetDeviceState(256, (LPVOID)&m_keyState)))
			break;
		if (result != DIERR_INPUTLOST && result != DIERR_NOTACQUIRED)
			return;

		// フォーカスが失われた場合は、デバイスを再取得します。
		if (FAILED(m_keyboard->Acquire()))
			return;
	}

	// 成功するか、不明なエラーが返されるまで、マウスをポーリングします。
	while (true)
	{
		m_mouse->Poll();
		if (SUCCEEDED(result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
			break;
		if (result != DIERR_INPUTLOST && result != DIERR_NOTACQUIRED)
			return;

		// フォーカスが失われた場合は、デバイスを再取得します。
		if (FAILED(m_mouse->Acquire()))
			return;
	}

	// マウスの相対位置を取得します。
	GetCursorPos(&m_position);
	ScreenToClient(m_window, &m_position);

	// プレススタンプ増加
	m_pressStamp++;
}

//-----------------------------------------------------------------------------
// 指定されたキーが押された場合にtrueを返します。
// プレススタンプを使用すると、長押しはfalseを返します。
//-----------------------------------------------------------------------------
bool Input::GetKeyPress(char key, bool ignorePressStamp)
{
	if ((m_keyState[key] & 0x80) == false)
		return false;

	bool pressed = true;

	if (ignorePressStamp == false)
		if (m_keyPressStamp[key] == m_pressStamp - 1 || m_keyPressStamp[key] == m_pressStamp)
			pressed = false;

	m_keyPressStamp[key] = m_pressStamp;

	return pressed;
}

//-----------------------------------------------------------------------------
// 指定されたキーが押された場合にtrueを返します。
// プレススタンプを使用すると、長押しはfalseを返します。
//-----------------------------------------------------------------------------
bool Input::GetButtonPress(char button, bool ignorePressStamp)
{
	if ((m_mouseState.rgbButtons[button] & 0x80) == false)
		return false;

	bool pressed = true;

	if (ignorePressStamp == false)
		if (m_buttonPressStamp[button] == m_pressStamp - 1 || m_buttonPressStamp[button] == m_pressStamp)
			pressed = false;

	m_buttonPressStamp[button] = m_pressStamp;

	return pressed;
}

//-----------------------------------------------------------------------------
// マウスの座標を返します。
//-----------------------------------------------------------------------------
POINT* Input::GetMousePos()
{
	return &m_position;
}


//-----------------------------------------------------------------------------
// マウスのx座標を返します。
//-----------------------------------------------------------------------------
long Input::GetPosX()
{
	return m_position.x;
}

//-----------------------------------------------------------------------------
// マウスのy座標を返します。
//-----------------------------------------------------------------------------
long Input::GetPosY()
{
	return m_position.y;
}

//-----------------------------------------------------------------------------
// マウスのx座標の変化を返します。
//-----------------------------------------------------------------------------
long Input::GetDeltaX()
{
	return m_mouseState.lX;
}

//-----------------------------------------------------------------------------
// マウスのy座標の変化を返します。
//-----------------------------------------------------------------------------
long Input::GetDeltaY()
{
	return m_mouseState.lY;
}

//-----------------------------------------------------------------------------
// マウスのスクロールホイールの変化を返します。
//-----------------------------------------------------------------------------
long Input::GetDeltaWheel()
{
	return m_mouseState.lZ;
}

