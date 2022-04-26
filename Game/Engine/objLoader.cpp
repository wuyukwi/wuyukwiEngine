/*
   Demo Name:  Game Project 11
      Author:  Allen Sherrod
     Chapter:  Chapter 12
*/


#include"objLoader.h"


stObjModel *LoadOBJModel(char *fileName)
{
   FILE *file;
   char *data = NULL;
   CToken lexer, tempLex;
   char tempLine[512];
   char token[512];

   // �t�@�C�����J���܂��B
   file = fopen(fileName, "r");
   if(!file) return NULL;

    // �t�@�C���̒������擾���܂��B
   fseek(file, 0, SEEK_END);
   int length = ftell(file);
   fseek(file, 0, SEEK_SET);

   // �t�@�C�����炷�ׂẴf�[�^��ǂݍ��݂܂�
   data = new char[(length + 1) * sizeof(char)];
   if(!data) return NULL;
   fread(data, length, 1, file);
   data[length] = '\0';

   // ����������t�@�C������܂��B
   fclose(file);

   // �t�@�C����TokenStream�ɐݒ肵�܂��B
   lexer.SetTokenStream(data);

   // �����K�v����܂���B
   delete[] data; data = NULL;

   bool validFile = false;

   // �t�@�C�����̂ǂ�����Wavefront�Ƃ����P���T���āA����.obj���݊��������邩�ǂ����𔻒f���܂��B
   // ����́A���f���[���킸���ɈقȂ�`���ɃG�N�X�|�[�g���邽�߂ł��B
   while(lexer.GetNextToken(token))
      {
         if(strcmp(token, "Wavefront") == 0)
            {
               validFile = true;
               break;
            }
      }

   if(!validFile) return NULL;

   // ���Z�b�g���܂��B
   lexer.Reset();

   // �t�@�C���Ő錾���ꂽ���ꂼ��̑������擾���邽�߂Ɏg�p����܂��B
   // faces�̓C���f�b�N�X���g�p���邽�߁A�����̐��͈قȂ�\��������܂��B
   int totalVertices = 0, totalNormals = 0,
       totalTexC = 0, totalFaces = 0;

   // �ŏ��́i�܂��͎��́j�s���擾���܂��B
   while(lexer.MoveToNextLine(tempLine))
      {
         // //�s��TokenStream�ɐݒ肵�܂��B
         tempLex.SetTokenStream(tempLine);

         // ���s��ǂݎ��܂��B
         lexer.GetNextToken(NULL);

         // temp lex�ɉ������ݒ肳��Ă���ꍇ�́A���s���܂��B
         if(!tempLex.GetNextToken(token)) continue;

         //�s�̍ŏ��̃g�[�N����v�Avn�Avt�A�܂���f�̏ꍇ ���̃J�E���^�[�𑝉����܂��B
         if(strcmp(token, "v") == 0) totalVertices++;
         else if(strcmp(token, "vn") == 0) totalNormals++;
         else if(strcmp(token, "vt") == 0) totalTexC++;
         else if(strcmp(token, "f") == 0) totalFaces++;

         token[0] = '\0';
      }

   // �f�[�^��ێ����邽�߂̈ꎞ�X�y�[�X�����蓖�Ă܂��B 
   // ���ꂼ��3�̒l�iv�C���f�b�N�X/vt�C���f�b�N�X/vn�C���f�b�N�X�j������3�̒��_�����邽�߁Afaces��9�ł��B
   float *verts = new float[totalVertices * 3];
   float *norms = new float[totalNormals * 3];
   float *texC = new float[totalTexC * 2];
   int *faces = new int[totalFaces * 9];
   int vIndex = 0, nIndex = 0, tIndex = 0, fIndex = 0, index = 0;

   // �t�@�C���̐擪�Ɉړ����܂��B
   lexer.Reset();

   // ���ׂĂ���蒼���܂����A����̓f�[�^���擾���܂��B
   while(lexer.MoveToNextLine(tempLine))
      {
       // temp lex�ɐݒ肵�A�ߋ��̉��s��ǂݎ��Atoken���擾���܂��B
         tempLex.SetTokenStream(tempLine);
         lexer.GetNextToken(NULL);
         if(!tempLex.GetNextToken(token)) continue;

         // v�̏ꍇ�A���_x�Ay�Az���擾���܂��B
         if(strcmp(token, "v") == 0)
            {
               // x���擾���ĕۑ����܂��B
               tempLex.GetNextToken(token);
               verts[vIndex] = (float)atof(token);
               vIndex++;
               
               // y���擾���ĕۑ����܂��B
               tempLex.GetNextToken(token);
               verts[vIndex] = (float)atof(token);
               vIndex++;
               
               // z���擾���ĕۑ����܂��B
               tempLex.GetNextToken(token);
               verts[vIndex] = (float)atof(token);
               vIndex++;
            }
         // ����ȊO�̏ꍇ�Avn�̏ꍇ�A���_�@����x�Ay�Az���擾���܂��B
         else if(strcmp(token, "vn") == 0)
            {
               // x���擾���ĕۑ����܂��B
               tempLex.GetNextToken(token);
               norms[nIndex] = (float)atof(token);
               nIndex++;
               
               // y���擾���ĕۑ����܂��B
               tempLex.GetNextToken(token);
               norms[nIndex] = (float)atof(token);
               nIndex++;
               
               // z���擾���ĕۑ����܂��B
               tempLex.GetNextToken(token);
               norms[nIndex] = (float)atof(token);
               nIndex++;
            }
         //����ȊO�̏ꍇvt�̏ꍇ�A�e�N�X�`�� u�Av���擾���܂��B
         else if(strcmp(token, "vt") == 0)
            {
               // u���擾���ĕۑ����܂��B
               tempLex.GetNextToken(token);
               texC[tIndex] = (float)atof(token);
               tIndex++;
               
               // v���擾���ĕۑ����܂��B
               tempLex.GetNextToken(token);
               texC[tIndex] = (float)atof(token);
               tIndex++;
            }
         //����ȊO�̏ꍇ�Af�̏ꍇ�A�e���_3�̃C���f�b�N�X�Z�b�g���擾���܂��B
         else if(strcmp(token, "f") == 0)
            {
               // Load for each vertex (3 in a triangle).
               for(int i = 0; i < 3; i++)
                  {
                   //���_���ƂɃ��[�h���܂��i�O�p�`��3�̒��_�j�B
                     tempLex.GetNextToken(token);
                     int len = strlen(token);

                     //�Z�b�g�i1/1/1�j�̊ԂɃX�y�[�X���Ȃ����߁A�g�[�N����P���ɓǂݎ�邱�Ƃ͂ł��Ȃ�����
                     // ���[�v����/�L���̑O�̊e�l�����o���K�v������܂��B
                     for(int s = 0; s < len + 1; s++)
                        {
                           char buff[64];

                           // /�łȂ��ꍇ�A�܂��͍Ō�ɂȂ��ꍇ�B
                           if(token[s] != '/' && s < len)
                              {
                                 buff[index] = token[s];
                                 index++;
                              }
                           else
                              {
                                 //����ȊO�̏ꍇ�́A��������I�����A�ϊ����ĕۑ����܂��B
                                 buff[index] = '\0';
                                 faces[fIndex] = (int)atoi(buff);
                                 fIndex++;
                                 index = 0;
                              }
                        }
                  }
            }

         token[0] = '\0';
      }

   // �����K�v����܂���B
   lexer.Shutdown();

   // ���蓖�Ăă��f���I�u�W�F�N�g���쐬���܂��B
   stObjModel *model = new stObjModel;
   if(!model) return NULL;
   memset(model, 0, sizeof(stObjModel));

   // faces�̐���ۑ����܂��B
   model->numFaces = totalFaces;

   // �ꎞ�J�E���^�[�����Z�b�g���܂��B
   vIndex = 0, nIndex = 0, tIndex = 0, fIndex = 0, index = 0;

   // ���f���̊e�����Ƀf�[�^�����蓖�Ă܂��B
   model->vertices = new float[totalFaces * 3 * 3];
   if(totalNormals) model->normals = new float[totalFaces * 3 * 3];
   if(totalTexC) model->texCoords = new float[totalFaces * 3 * 2];

   // ���[�v���ă��f���ɓ��͂��܂��B
   for(int f = 0; f < totalFaces * 9; f+=3)
      {
         // ���_���擾���܂��B �C���f�b�N�X��1����ő�ł͂Ȃ�0����ő�-1�͈̔͂ł���K�v�����邽�߁A1�����Z���܂��B
         // ���_�ɂ�3�̃R���|�[�l���g�ix�Ay�Az�j�����邽�߁A3���|���܂��B
         model->vertices[vIndex + 0] = verts[(faces[f + 0] - 1) * 3 + 0];
         model->vertices[vIndex + 1] = verts[(faces[f + 0] - 1) * 3 + 1];
         model->vertices[vIndex + 2] = verts[(faces[f + 0] - 1) * 3 + 2];
         vIndex += 3;

         //�e�N�X�`�����W�f�[�^�ł��������Ƃ��s���܂��B 2�Ԗڂ̃f�[�^��texcoord�f�[�^�Ȃ̂ŁA1��f�iv / vt / vn�j�ɓK�p���܂��B
         if(model->texCoords)
            {
               model->texCoords[tIndex + 0] = texC[(faces[f + 1] - 1) * 2 + 0];
               model->texCoords[tIndex + 1] = texC[(faces[f + 1] - 1) * 2 + 1];
               tIndex += 2;
            }

         // �@���̃f�[�^�ł��������Ƃ��s���܂��B
         if(model->normals)
            {
               model->normals[nIndex + 0] = norms[(faces[f + 2] - 1) * 3 + 0];
               model->normals[nIndex + 1] = norms[(faces[f + 2] - 1) * 3 + 1];
               model->normals[nIndex + 2] = norms[(faces[f + 2] - 1) * 3 + 2];
               nIndex += 3;
            }
      }

   // �ꎞ�f�[�^���폜���܂��B
   delete[] verts;
   delete[] norms;
   delete[] texC;
   delete[] faces;

   return model;
}


void FreeOBJModel(stObjModel *model)
{
   if(!model) return;

   // ���ׂẴ��\�[�X��������܂��B
   if(model->vertices) delete[] model->vertices;
   model->vertices = NULL;
   if(model->normals) delete[] model->normals;
   model->normals = NULL;
   if(model->texCoords) delete[] model->texCoords;
   model->texCoords = NULL;

   delete model;
   model = NULL;
}