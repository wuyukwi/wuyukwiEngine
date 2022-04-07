/*********************************************************************************

  *FileName: GUI.h
            �R�E  �L�K�N
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/05

**********************************************************************************/

#ifndef _GUI_H_
#define _GUI_H_

#include "defines.h"

struct stGUIControl
{
    // �{�^���^�C�v�A�J���[
    int m_type, m_id;
    unsigned long m_color;

    // �e�L�X�g�̏ꍇ�A����̓t�H���gID�ł���A����ȊO�̏ꍇ�͐ÓI�o�b�t�@�[ID�ł�
    int m_listID;

    // �I�u�W�F�N�g�̈ʒu�A�{�^���̕��ƍ���
    int m_xPos, m_yPos, m_width, m_height;

    // �e�L�X�g
    char* m_text;

    //�{�^���̃A�b�v�A�_�E���A�I�[�o�[�e�N�X�`���B m_upTex�͔w�i�摜�ɂ��g�p����܂�
    int m_upTex, m_downTex, m_overTex;
};


class CGUISystem
{
public:
    CGUISystem() : m_controls(0), m_totalControls(0),m_backDropID(-1) {}
    ~CGUISystem() { Shutdown(); }

    int IncreaseControls();

    bool AddBackdrop(int texID, int staticID);
    bool AddStaticText(int id, const char* text, int x, int y,unsigned long color, int fontID);
    bool AddButton(int id, int x, int y, int width,int height, int upID, int overID,int downID, int staticID);
    void Shutdown();

    stGUIControl* GetGUIControl(int id)
    {
        if (id < 0 || id >= m_totalControls) 
            return NULL;

        return &m_controls[id];
    }

    int GetTotalControls()
    {
        return m_totalControls;
    }

    stGUIControl* GetBackDrop()
    {
        if (m_backDropID >= 0 && m_totalControls)
            return &m_controls[m_backDropID];

        return NULL;
    }

private:
    stGUIControl* m_controls;
    int m_totalControls;
    int m_backDropID;
};

#endif // _GUI_H_