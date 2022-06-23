/*********************************************************************************

  *FileName: GUI.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/05

**********************************************************************************/

#ifndef _GUI_H_
#define _GUI_H_

#include "defines.h"
#include <vector>

struct stGUIControl
{
    // ボタンタイプ、カラー
    int m_type, m_id;
    unsigned long m_color;

    // テキストの場合、これはフォントIDであり、それ以外の場合は静的バッファーIDです
    int m_listID;

    // オブジェクトの位置、ボタンの幅と高さ
    int m_xPos, m_yPos, m_width, m_height;

    // テキスト
    char* m_text;

    //ボタンのアップ、ダウン、オーバーテクスチャ。 m_upTexは背景画像にも使用されます
    int m_upTex, m_downTex, m_overTex;
};

class CGUISystem
{
public:
    CGUISystem() : m_backDropID(-1) {}
    ~CGUISystem() { Shutdown(); }

    int IncreaseControls();

    bool AddBackdrop(int texID, int staticID);
    bool AddStaticText(int id, const char* text, int x, int y, unsigned long color, int fontID);
    bool AddButton(int id, int x, int y, int width, int height, int upID, int overID, int downID, int staticID);
    void Shutdown();

    stGUIControl* GetGUIControl(size_t id)
    {
        if (id < 0 || id >= m_controls.size())
            return nullptr;

        return &m_controls[id];
    }

    int GetTotalControls() const
    {
        return m_controls.size();
    }

    stGUIControl* GetBackDrop()
    {
        if (m_backDropID >= 0 && !m_controls.empty())
            return &m_controls[m_backDropID];

        return nullptr;
    }

private:

    std::vector<stGUIControl> m_controls{};
    int m_backDropID;
};

#endif // _GUI_H_