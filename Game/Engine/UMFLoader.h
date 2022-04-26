/*
   Demo Name:  Game Project 11
      Author:  Allen Sherrod
     Chapter:  Chapter 12

   Description:

      Load the website's ultimate model format (UMF).  This model format is
      meant to be extermely simple to read and write to.

   File format:
      id (must be 9004)
      number of meshes

      (For each mesh:)
      diffuse, specular, power
      num verts
      vert data

      num normals
      vert normals data

      num colors
      vert colors data

      num tex coords
      tex coord data

      number of faces
      face data

      num bone indices
      bone indices

      num bones
      bones

      number of faces
      face data
*/


#ifndef _UMF_LOADER_H_
#define _UMF_LOADER_H_

#include<stdio.h>
#include<math.h>
#include"Matrix.h"


struct stTexCoord
{
   stTexCoord() : tu(0), tv(0) {}
   // tu, tv texture coordinates.
   float tu, tv;
};

struct stFace
{
   stFace() { indices[0] = indices[1] = indices[2] = 0; }

   // Vertex indexes and a surface normal.
   unsigned int indices[3];
   Vector3f normal;
};

struct stKeyFrame
{
   stKeyFrame() : time(0) {}

   float time;
   Vector3f rotation, translation;
};

struct stBone
{
   stBone() : id(0), totalFrames(0), currentFrame(0),
              keyFrames(0), parentID(0){}

   char name[32];
   unsigned long id;
   char parent[32];
   int parentID;

   Matrix4x4f relative, absolute;
   int currentFrame, totalFrames;
   stKeyFrame *keyFrames;
};

struct stUMFModel
{
   stUMFModel() : vertices(0), faces(0), normals(0), colors(0), boneIndices(0),
                  bones(0), totalBones(0), totalVertices(0),
                  totalFaces(0), power(0), texCoords(0) {}

   // Material data.
   Vector3f diffuse;
   Vector3f specular;
   int power;

   // Model data.
   Vector3f *vertices;
   stTexCoord *texCoords;
   Vector3f *normals;
   Vector3f *colors;
   int *boneIndices;
   stBone *bones;
   stFace *faces;

   // Array counters.
   int totalVertices;
   int totalBones;
   int totalFaces;

   // Bounding box data.
   Vector3f bbMin, bbMax;
};


bool SaveUMF(char *file, stUMFModel *meshes, int numMeshes);
bool LoadUMF(char *file, stUMFModel **model, int *totalModels);
void FreeModel(stUMFModel *mesh);
void FreeModel(stUMFModel *meshes, int numMeshes);

#endif