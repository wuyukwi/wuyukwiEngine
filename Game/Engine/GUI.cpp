/*********************************************************************************

  *FileName: GUI.cpp
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/05

**********************************************************************************/

#include "GUI.h"

bool CGUISystem::AddBackdrop(int texID, int sID)
{
    if (texID < 0 || sID < 0)
        return false;

    if (m_backDropID < 0)
    {
        stGUIControl control{};

        // 必要な情報を保存
        control.m_type = UGP_GUI_BACKDROP;
        control.m_upTex = texID;
        control.m_listID = sID;

        //どのインデックスが背景であるかを追跡用
        m_backDropID = m_controls.size();

        this->m_controls.push_back(control);
    }
    else
    {
        //// IDを保存するだけ
        m_controls[m_backDropID].m_upTex = texID;
        m_controls[m_backDropID].m_listID = sID;
    }

    return true;
}

bool CGUISystem::AddStaticText(int id, const char* text, int x, int y, unsigned long color, int fontID)
{
    if (!text || fontID < 0)
        return false;

    stGUIControl control{};

    // テキスト必要の情報を保存
    control.m_type = UGP_GUI_STATICTEXT;
    control.m_id = id;
    control.m_color = color;
    control.m_xPos = x;
    control.m_yPos = y;
    control.m_listID = fontID;

    // データをコピーする
    int len = strlen(text);
    control.m_text = new char[len + 1];

    if (!control.m_text)
        return false;

    memcpy(control.m_text, text, len);
    control.m_text[len] = '\0';

    this->m_controls.push_back(control);

    return true;
}

bool CGUISystem::AddButton(int id, int x, int y, int width, int height, int upID, int overID, int downID, int staticID)
{
    stGUIControl control{};

    //  ボタン必要の情報を保存
    control.m_type = UGP_GUI_BUTTON;
    control.m_id = id;
    control.m_xPos = x;
    control.m_yPos = y;
    control.m_width = width;
    control.m_height = height;
    control.m_upTex = upID;
    control.m_overTex = overID;
    control.m_downTex = downID;
    control.m_listID = staticID;

    this->m_controls.push_back(control);

    return true;
}

void CGUISystem::Shutdown()
{
    // 全てのリソースをリリース
    for (uint32_t s = 0; s < m_controls.size(); s++)
    {
        SAFE_DELETE_ARRAY(m_controls[s].m_text);
    }
}