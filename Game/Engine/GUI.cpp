/*********************************************************************************

  *FileName: GUI.cpp
            �R�E  �L�K�N
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/05

**********************************************************************************/

#include "GUI.h"

// m_control�̔z��𑝂₷
int CGUISystem::IncreaseControls()
{
    if (!m_controls)
    {
        m_controls = new stGUIControl[1];

        if (!m_controls) 
            return UGP_FAIL;

        memset(&m_controls[0], 0, sizeof(stGUIControl));
    }
    else
    {
        stGUIControl* temp;
        temp = new stGUIControl[m_totalControls + 1];

        if (!temp) 
            return UGP_FAIL;

        memset(temp, 0, sizeof(stGUIControl) *(m_totalControls + 1));

        memcpy(temp, m_controls,sizeof(stGUIControl) * m_totalControls);

        delete[] m_controls;
        m_controls = temp;
    }

    return UGP_OK;
}


bool CGUISystem::AddBackdrop(int texID, int sID)
{
    if (texID < 0 || sID < 0) 
        return false;

    if (m_backDropID < 0)
    {
         // �V�����R���g���[���[�z����쐬
        if (!IncreaseControls()) 
            return false;

        // �K�v�ȏ���ۑ�
        m_controls[m_totalControls].m_type = UGP_GUI_BACKDROP;
        m_controls[m_totalControls].m_upTex = texID;
        m_controls[m_totalControls].m_listID = sID;

        //�ǂ̃C���f�b�N�X���w�i�ł��邩��ǐ՗p
        m_backDropID = m_totalControls;

        m_totalControls++;
    }
    else
    {
        //// ID��ۑ����邾��
        m_controls[m_backDropID].m_upTex = texID;
        m_controls[m_backDropID].m_listID = sID;
    }

    return true;
}


bool CGUISystem::AddStaticText(int id, const char* text, int x, int y,unsigned long color, int fontID)
{
    if (!text || fontID < 0) 
        return false;

    // �V�����R���g���[���[�z����쐬
    if (!IncreaseControls()) 
        return false;

    // �e�L�X�g�K�v�̏���ۑ�
    m_controls[m_totalControls].m_type = UGP_GUI_STATICTEXT;
    m_controls[m_totalControls].m_id = id;
    m_controls[m_totalControls].m_color = color;
    m_controls[m_totalControls].m_xPos = x;
    m_controls[m_totalControls].m_yPos = y;
    m_controls[m_totalControls].m_listID = fontID;

    // �f�[�^���R�s�[����
    int len = strlen(text);
    m_controls[m_totalControls].m_text = new char[len + 1];

    if (!m_controls[m_totalControls].m_text) 
        return false;

    memcpy(m_controls[m_totalControls].m_text, text, len);
    m_controls[m_totalControls].m_text[len] = '\0';

    m_totalControls++;

    return true;
}


bool CGUISystem::AddButton(int id, int x, int y, int width,int height, int upID, int overID,int downID, int staticID)
{
    // �V�����R���g���[���[�z����쐬
    if (!IncreaseControls()) 
        return false;

    //  �{�^���K�v�̏���ۑ�
    m_controls[m_totalControls].m_type = UGP_GUI_BUTTON;
    m_controls[m_totalControls].m_id = id;
    m_controls[m_totalControls].m_xPos = x;
    m_controls[m_totalControls].m_yPos = y;
    m_controls[m_totalControls].m_width = width;
    m_controls[m_totalControls].m_height = height;
    m_controls[m_totalControls].m_upTex = upID;
    m_controls[m_totalControls].m_overTex = overID;
    m_controls[m_totalControls].m_downTex = downID;
    m_controls[m_totalControls].m_listID = staticID;

    m_totalControls++;

    return true;
}


void CGUISystem::Shutdown()
{
    // �S�Ẵ��\�[�X�������[�X
    for (int s = 0; s < m_totalControls; s++)
    {   
        SAFE_DELETE_ARRAY(m_controls[s].m_text);
    }

    m_totalControls = 0;

    SAFE_DELETE_ARRAY(m_controls);
  
}