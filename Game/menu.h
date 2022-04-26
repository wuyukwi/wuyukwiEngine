/*********************************************************************************

  *FileName: menu.h
            �R�E  �L�K�N
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/19

**********************************************************************************/

#ifndef _MENU_H_
#define _MENU_H_

//-----------------------------------------
// ���j���[�֐�
//-----------------------------------------
bool InitializeMainMenu();
void MainMenuCallback(int id, size_t state);
void MainMenuRender();


// ���C�����j���[
#define GUI_MAIN_SCREEN       (1)
#define GUI_START_SCREEN      (2)
#define GUI_CREDITS_SCREEN    (3)
#define GUI_LOADING_SCREEN    (4)

// GUI�R���g���[���[
#define STATIC_TEXT_ID     (1)
#define BUTTON_START_ID    (2)
#define BUTTON_CREDITS_ID  (3)
#define BUTTON_QUIT_ID     (4)
#define BUTTON_BACK_ID     (5)
#define BUTTON_LEVEL_1_ID  (6)

// GUI ids.
extern int g_mainGui;
extern int g_startGui;
extern int g_creditsGui;
extern int g_loadingGui;
extern int g_currentGUI;

// Sound id.
extern int g_menuSound;

#endif _MENU_H_