/*********************************************************************************

  *FileName: stGameObject.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/22

**********************************************************************************/

#ifndef _ST_GAME_OBJECT_H_
#define _ST_GAME_OBJECT_H_

#define X_MESH          1
#define X_ANIMATED_MESH 2

struct stGameObject
{
    stGameObject() : m_type(0), m_id(0), m_x(0), m_y(0), m_z(0),
        m_rotX(0), m_rotY(0), m_rotZ(0), m_visible(1) {}

    int m_type;
    int m_id;
    float m_x, m_y, m_z;
    float m_rotX, m_rotY, m_rotZ;

    int m_visible;

    Matrix4x4f worldMat;
};

#endif // _ST_GAME_OBJECT_H_
