/*********************************************************************************

  *FileName: BoundingGeometry.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/12

**********************************************************************************/


#ifndef _BOUNDINGGEOMETRY_H_
#define _BOUNDINGGEOMETRY_H_


//class CBoundingBase
//{
//   public:
//      CBoundingBase(){}
//      virtual ~CBoundingBase() {}
//
//      virtual void CreateFromPoints(Vector3f *pointList, int numPoints) = 0;
//      virtual bool isPointInside(Vector3f &v) = 0;
//
//      virtual bool Intersect(CRay ray, float *dist) = 0;
//      virtual bool Intersect(CRay ray, float length, float *dist) = 0;
//
//      virtual void GetPlanes(CPlane *planes) = 0;
//      virtual bool isRayInside(CRay &ray, float length) = 0;
//};
//
//
//class CBoundingBox : public CBoundingBase
//{
//   public:
//      CBoundingBox() {}
//      ~CBoundingBox() {}
//      
//      void CreateFromPoints(Vector3f *pointList, int numPoints);
//      bool isPointInside(Vector3f &v);
//
//      bool Intersect(CRay ray, float *dist);
//      bool Intersect(CRay ray, float length, float *dist);
//
//      void GetPlanes(CPlane *planes);
//      bool isRayInside(CRay &ray, float length);
//      
//      Vector3f m_min, m_max;
//};
//
//
//class CBoundingSphere : public CBoundingBase
//{
//   public:
//      CBoundingSphere() : m_radius(0) {}
//      ~CBoundingSphere() {}
//      
//      void CreateFromPoints(Vector3f *pointList, int numPoints);
//      bool isPointInside(Vector3f &v);
//
//      bool Intersect(CRay ray, float *dist);
//      bool Intersect(CRay ray, float length, float *dist);
//
//      void GetPlanes(CPlane *planes) {}
//      bool isRayInside(CRay &ray, float length);
//      
//      Vector3f m_center;
//      float m_radius;
//};
//
//bool BoxToBoxIntersect(CBoundingBox &bb1, CBoundingBox &bb2);
//bool SphereToSphereIntersect(CBoundingSphere &bs1, CBoundingSphere &bs2);

#endif _BOUNDINGGEOMETRY_H_