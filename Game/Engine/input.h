/********************************************************************************* 

  *FileName: input.cpp  
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/01/28

**********************************************************************************/  

#ifndef INPUT_H_
#define INPUT_H_

//-----------------------------------------------------------------------------
// Input Class
//-----------------------------------------------------------------------------
class Input
{
public:
	Input(HWND window);
	virtual ~Input();

	void Update();

	bool GetKeyPress(char key, bool ignorePressStamp = false);
	bool GetButtonPress(char button, bool ignorePressStamp = false);

	long GetPosX();
	long GetPosY();
	long GetDeltaX();
	long GetDeltaY();
	long GetDeltaWheel();

	POINT* GetMousePos();

private:
	HWND	       m_window;		   // 親ウィンドウのハンドル。
	IDirectInput8* m_di;			   // DirectInputオブジェクト。
	size_t  m_pressStamp; 		   // 現在のプレススタンプ（フレームごとにインクリメント）。

	IDirectInputDevice8* m_keyboard;	   // DirectInputキーボードデバイス。
	char		     m_keyState[256];	   // キーボードキーの状態を保存用。
	size_t	     m_keyPressStamp[256]; // 各キーが事前にシードされた最後のフレームにスタンプを付けます。

	IDirectInputDevice8* m_mouse;		    // DirectInputマウスデバイス。
	DIMOUSESTATE	     m_mouseState;	    // マウスボタンの状態を保存します。
	size_t	     m_buttonPressStamp[3]; // 各ボタンがプレシードされた最後のフレームにスタンプを押します。
	POINT		     m_position;	    // 画面上のマウスの位置を保存用。
};


#endif // INPUT_H_