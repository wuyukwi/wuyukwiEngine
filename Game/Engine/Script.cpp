/*********************************************************************************

  *FileName: Script.cpp
            �R�E  �L�K�N
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/11

**********************************************************************************/

#include"Script.h"
#include<fstream>


CScript::CScript() : totalScriptLines(0), currentLine(0),currentLineChar(0), m_script(0)
{

}


CScript::~CScript()
{
   Shutdown();
}


bool CScript::LoadScriptFile(const char *filename)
{
   std::ifstream input, input2;
   char tempLine[MAX_LINE_SIZE];
   
   input.open(filename);
   if(!input.is_open()) 
       return false;
   
   Shutdown();

   // �e�L�X�g�t�@�C�����J���s���Ǝ擾����
   while(!input.eof())
      {
         input.getline(tempLine, MAX_LINE_SIZE, '\n');
         totalScriptLines++;
      }
   
   input.close();
   
   input2.open(filename);
   if(!input2.is_open()) 
       return false;
   
   // �e�L�X�g�̂��ׂĂ̍s�����[�h����
   m_script = new char*[totalScriptLines];
   
   for(int i = 0; i < totalScriptLines; i++)
      {
         m_script[i] = new char[MAX_LINE_SIZE + 1];
         input2.getline(m_script[i], MAX_LINE_SIZE, '\n');
      }
   
   input2.close();
   return true;
}


void CScript::ParseCommand(char *destCommand)
{
    //���̊֐��́A���݂̍s�̍ŏ��̒P����擾���A�����destCommand�ɕۑ����܂��B

   int commandSize = 0;

   // destcommand��NULL�̏ꍇ�A�s���I�[�o�[���Ă���ꍇ�A�܂��͌��݂̍s�̏I���ɂ���ꍇ�́A�߂�܂��B
   if(!destCommand) 
       return;
   if(currentLine >= totalScriptLines) 
       return;
   if(currentLineChar >= (int)strlen(m_script[currentLine])) 
       return;

   // �z��̏�����
   destCommand[0] = '\0';
   
   // �s�̐擪�Ɂ�������ꍇ�A����̓R�����g�ł���
   if(IsLineComment())
      {
         destCommand[0] = '#';
         destCommand[1] = '\0';
         return;
      }

   // �X�y�[�X�܂��͉��s��������܂ł��ׂĂ̕�����ǂݍ��݂܂��B
   while(currentLineChar < (int)strlen(m_script[currentLine]))
      {
         if(m_script[currentLine][currentLineChar] == ' ' ||
            m_script[currentLine][currentLineChar] == '\n')
            break;

         // �z��Ƀe�L�X�g�f�[�^��ۑ����܂�
         destCommand[commandSize] = m_script[currentLine][currentLineChar];
         commandSize++;
         currentLineChar++;
      }

   // ���̃f�[�^�܂ŃX�L�b�v���܂��B
   currentLineChar++;

   while (currentLineChar < (int)strlen(m_script[currentLine]))
   {
       if (m_script[currentLine][currentLineChar] != ' ')
       {
           break;
       }
       
       currentLineChar++;
   }
   destCommand[commandSize] = '\0';
}

void CScript::ParseStringParam(char *destString)
{
   // ���̊֐��́g �h�̒��Ƀe�L�X�g�����A�����destString�ɕۑ����܂��B

   int paramSize = 0;
   bool beginQuoteFound = false;
   bool endQuoteFound = false;

   // destcommand��NULL�̏ꍇ�A�s���I�[�o�[���Ă���ꍇ�A�܂��͌��݂̍s�̏I���ɂ���ꍇ�́A�߂�܂��B
   if(!destString) 
       return;
   if(currentLine >= totalScriptLines) 
       return;
   if(currentLineChar >= (int)strlen(m_script[currentLine])) 
       return;

   // ������z������������܂�
   destString[0] = '\0';

   // �ŏ���"���X�L�b�v���܂��B
   currentLineChar++;

    // "�܂��͉��s��������܂ł��ׂĂ̕�����ǂݍ��݂܂��B
    while (currentLineChar < (int)strlen(m_script[currentLine]))
    {
        if (m_script[currentLine][currentLineChar] == '"')
        {
            endQuoteFound = true;
            break;
        }

        if (m_script[currentLine][currentLineChar] == '\n')
        {
            break;
        }

        // �e�L�X�g�f�[�^��ۑ�
        destString[paramSize] = m_script[currentLine][currentLineChar];
        paramSize++;
        currentLineChar++;
    }

    // �I�����p���Ǝ��̃X�y�[�X�܂��͉��s���X�L�b�v���܂��B�����񂪕����s���߂邱�Ƃ͋�����Ă��܂���B 
    if (endQuoteFound)
        currentLineChar += 2;
    else
        currentLineChar++;
  

   destString[paramSize] = '\0';
}


bool CScript::ParseBoolParam()
{
   // ���̊֐��͎��̃e�L�X�g���擾���A���̃e�L�X�g���utrue�v�̏ꍇ��true��Ԃ��A�ufalse�v�̏ꍇ��false��Ԃ��܂��B

   char string[MAX_PARAM_SIZE];
   int paramSize = 0;

   // �s������Ȃ��Ȃ����ꍇ�A�܂��͌��݂̍s�̏I���ɂ���ꍇ�́A�߂�܂��B
   if(currentLine >= totalScriptLines) 
       return false;
   if(currentLineChar >= (int)strlen(m_script[currentLine])) 
       return false;

   // �X�y�[�X�܂��͉��s��������܂ŁA���ׂĂ̕�����ǂݍ��݂܂�
   while(currentLineChar < (int)strlen(m_script[currentLine]))
      {
       if (m_script[currentLine][currentLineChar] == ' ' ||
           m_script[currentLine][currentLineChar] == '\n')
       {
           break;
       }
           

         // �f�[�^��ۑ�
         string[paramSize] = m_script[currentLine][currentLineChar];
         paramSize++;
         currentLineChar++;
      }

   // ���̃X�y�[�X�܂��͉��s���X�L�b�v���܂��B
   currentLineChar++;
   string[paramSize] = '\0';

   if(_stricmp(string, "true") == 0) 
       return true;

   return false;
}


int CScript::ParseIntParam()
{
   // ���̊֐��͎��̃e�L�X�g���󂯎��A�����int�ɕϊ����܂��B

   char string[MAX_PARAM_SIZE];
   int paramSize = 0;

   // �s������Ȃ��Ȃ����ꍇ�A�܂��͌��݂̍s�̏I���ɂ���ꍇ�́A�߂�܂��B
   if(currentLine >= totalScriptLines) 
       return false;
   if(currentLineChar >= (int)strlen(m_script[currentLine])) 
       return false;

   // �X�y�[�X�܂��͉��s��������܂ŁA���ׂĂ̕�����ǂݍ��݂܂�
   while(currentLineChar < (int)strlen(m_script[currentLine]))
      {
         if(m_script[currentLine][currentLineChar] == ' ' ||
            m_script[currentLine][currentLineChar] == '\n')
            break;

         // �f�[�^��ۑ�
         string[paramSize] = m_script[currentLine][currentLineChar];
         paramSize++;
         currentLineChar++;
      }

   // ���̃X�y�[�X�܂��͉��s���X�L�b�v���܂��B
   currentLineChar++;
   string[paramSize] = '\0';
   return atoi(string);
}


float CScript::ParseFloatParam()
{
    // ���̊֐��͎��̃e�L�X�g���󂯎��A�����float�ɕϊ����܂��B

   char string[MAX_PARAM_SIZE];
   int paramSize = 0;

   // �s������Ȃ��Ȃ����ꍇ�A�܂��͌��݂̍s�̏I���ɂ���ꍇ�́A�߂�܂��B
   if(currentLine >= totalScriptLines) 
       return false;
   if(currentLineChar >= (int)strlen(m_script[currentLine])) 
       return false;

   // �X�y�[�X�܂��͉��s��������܂ŁA���ׂĂ̕�����ǂݍ��݂܂�
   while(currentLineChar < (int)strlen(m_script[currentLine]))
      {
         if(m_script[currentLine][currentLineChar] == ' ' ||
            m_script[currentLine][currentLineChar] == '\n')
            break;

         // �f�[�^��ۑ�
         string[paramSize] = m_script[currentLine][currentLineChar];
         paramSize++;
         currentLineChar++;
      }

   // ���̃X�y�[�X�܂��͉��s���X�L�b�v���܂��B
   currentLineChar++;
   string[paramSize] = '\0';
   return (float)atof(string);
}

void CScript::ParseSoundFileParan(char* destString,int* repeats)
{
    MoveToNextLine();

    // ���̊֐��́g �h�̒��Ƀe�L�X�g�����A�����destString�ɕۑ����܂��B
    char stringInt[MAX_PARAM_SIZE];
    int paramSizeInt = 0;

    int paramSize = 0;
    bool beginQuoteFound = false;
    bool endQuoteFound = false;

    // destcommand��NULL�̏ꍇ�A�s���I�[�o�[���Ă���ꍇ�A�܂��͌��݂̍s�̏I���ɂ���ꍇ�́A�߂�܂��B
    if (!destString||!repeats)
        return;
    if (currentLine >= totalScriptLines)
        return;
    if (currentLineChar >= (int)strlen(m_script[currentLine]))
        return;

    // ������z������������܂�
    destString[0] = '\0';

    // �ŏ���"���X�L�b�v���܂��B
    currentLineChar++;

    // "�܂��͉��s��������܂ł��ׂĂ̕�����ǂݍ��݂܂��B
    while (currentLineChar < (int)strlen(m_script[currentLine]))
    {
        if (m_script[currentLine][currentLineChar] == '"')
        {
            endQuoteFound = true;

            break;
        }

        if (m_script[currentLine][currentLineChar] == '\n')
        {
            break;
        }

        // �e�L�X�g�f�[�^��ۑ�
        destString[paramSize] = m_script[currentLine][currentLineChar];
        paramSize++;
        currentLineChar++;
    }
    // �I�����p���Ǝ��̃X�y�[�X�܂��͉��s���X�L�b�v���܂��B�����񂪕����s���߂邱�Ƃ͋�����Ă��܂���B 
    if (endQuoteFound)
        currentLineChar += 2;
    else
        currentLineChar++;
    destString[paramSize] = '\0';


    while (currentLineChar < (int)strlen(m_script[currentLine]))
    {
        // �ϐ���ǂݍ���
        if (m_script[currentLine][currentLineChar] != ' ')
        {
            // �f�[�^��ۑ�
            stringInt[paramSizeInt] = m_script[currentLine][currentLineChar];
            paramSizeInt++;
            currentLineChar++;
        }
        else
        {
            break;
        }
    }

    stringInt[paramSize] = '\0';
    *repeats= atoi(stringInt);
}

void CScript::MoveToStart()
{
   currentLine = 0;
   currentLineChar = 0;
}


void CScript::MoveToNextLine()
{
   currentLine++;
   currentLineChar = 0;
}


int CScript::GetCurrentLineNum()
{
   return currentLine;
}


int CScript::GetTotalLines()
{
   return totalScriptLines;
}


bool CScript::IsLineComment()
{
   // �s�̍ŏ��̕��������̏ꍇ�A���̍s�̓R�����g�A�E�g����K�v������܂��B
   if(m_script[currentLine][0] == '#') 
       return true;

   return false;
}


void CScript::Shutdown()
{
   if(m_script)
      {
         // �X�N���v�g�t�@�C���̂��ׂẴf�[�^���폜���܂��B
         for(int i = 0; i < totalScriptLines; i++)
            {
               if(m_script[i])
                  {
                     delete[] m_script[i];
                     m_script[i] = 0;
                  }
            }
         
         delete m_script;
         m_script = 0;
      }
 
   // �ϐ������������܂�
   totalScriptLines = 0;
   currentLineChar = 0;
   currentLine = 0;
}

//-----------------------------------------------------------------------------
// �X�N���v�g�t�@�C����ۑ�
//-----------------------------------------------------------------------------
bool CScript::SaveScript(const char* filename, const char** data, int currentLine, int currentLineChar)
{
    //std::ofstream output, output2;
    //char tempLine[MAX_LINE_SIZE];

    //output.open(filename, std::ios::trunc);
    //if (!output.is_open())
    //    return false;

    //Shutdown();

    //for (int i = 0; i < currentLine; i++)
    //{
    //    output << data[i][0];
    //    output.write
    //}
   

    //while (!output.eof())
    //{
    //   

    //    output.getline(tempLine, MAX_LINE_SIZE, '\n');
    //    totalScriptLines++;
    //}

    //output.close();

    //output2.open(filename);
    //if (!output2.is_open())
    //    return false;

    //// �e�L�X�g�̂��ׂĂ̍s�����[�h����
    //m_script = new char* [totalScriptLines];

    //for (int i = 0; i < totalScriptLines; i++)
    //{
    //    m_script[i] = new char[MAX_LINE_SIZE + 1];
    //    output2.getline(m_script[i], MAX_LINE_SIZE, '\n');
    //}

    //output2.close();
    //return true;
    //FILE* file = NULL;
    //char output[260];

    //// �t�@�C�����Ȃ��ꍇ�͌��L�̃t�@�C��������
    //if (filename != NULL)
    //{
    //    if ((file = fopen(filename, "w")) == NULL)
    //        return;
    //}
    //else
    //{
    //    if ((file = fopen(GetFilename(), "w")) == NULL)
    //        return;
    //}

    //// #begin ����������
    //fputs("#begin\n", file);

    //// �S�Ă̕ϐ�����������
    //m_variables->Iterate(true);
    //while (m_variables->Iterate() != NULL)
    //{
    //    switch (m_variables->GetCurrent()->GetType())
    //    {
    //    case VARIABLE_BOOL:
    //        if (*((bool*)m_variables->GetCurrent()->GetData()) == true)
    //            sprintf(output, "%s bool true", m_variables->GetCurrent()->GetName());
    //        else
    //            sprintf(output, "%s bool false", m_variables->GetCurrent()->GetName());
    //        fputs(output, file);
    //        fputs("\n", file);
    //        continue;

    //    case VARIABLE_COLOUR:
    //        sprintf(output, "%s colour %f %f %f %f", m_variables->GetCurrent()->GetName(), ((D3DCOLORVALUE*)m_variables->GetCurrent()->GetData())->r, ((D3DCOLORVALUE*)m_variables->GetCurrent()->GetData())->g, ((D3DCOLORVALUE*)m_variables->GetCurrent()->GetData())->b, ((D3DCOLORVALUE*)m_variables->GetCurrent()->GetData())->a);
    //        fputs(output, file);
    //        fputs("\n", file);
    //        continue;

    //    case VARIABLE_FLOAT:
    //        sprintf(output, "%s float %f", m_variables->GetCurrent()->GetName(), *(float*)m_variables->GetCurrent()->GetData());
    //        fputs(output, file);
    //        fputs("\n", file);
    //        continue;

    //    case VARIABLE_NUMBER:
    //        sprintf(output, "%s number %d", m_variables->GetCurrent()->GetName(), *(long*)m_variables->GetCurrent()->GetData());
    //        fputs(output, file);
    //        fputs("\n", file);
    //        continue;

    //    case VARIABLE_STRING:
    //        sprintf(output, "%s string \"%s\"", m_variables->GetCurrent()->GetName(), (char*)m_variables->GetCurrent()->GetData());
    //        fputs(output, file);
    //        fputs("\n", file);
    //        continue;

    //    case VARIABLE_VECTOR:
    //        sprintf(output, "%s vector %f %f %f", m_variables->GetCurrent()->GetName(), ((D3DXVECTOR3*)m_variables->GetCurrent()->GetData())->x, ((D3DXVECTOR3*)m_variables->GetCurrent()->GetData())->y, ((D3DXVECTOR3*)m_variables->GetCurrent()->GetData())->z);
    //        fputs(output, file);
    //        fputs("\n", file);
    //        continue;

    //    default:
    //        sprintf(output, "%s unknown %s", m_variables->GetCurrent()->GetName(), (char*)m_variables->GetCurrent()->GetData());
    //        fputs(output, file);
    //        fputs("\n", file);
    //        continue;
    //    }
    //}

    //// #end ����������
    //fputs("#end", file);

    //// �t�@�C�������
    //fclose(file);

return true;
}