/*********************************************************************************

  *FileName: PropertyScript.cpp
            �R�E  �L�K�N
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/11

**********************************************************************************/


#include"PropertyScript.h"
#include<fstream>

using std::ifstream;

int DetermineType(int startIndex, char* buffer)
{
    int numComponents = 0;
    int type = NULL_VAR;
    bool decimalFound = false;
    bool charFound = false;
    int index = startIndex;

    // ���[�v���āA��������擾���܂��B
    while (index < (int)strlen(buffer))
    {
        // �V�����s���Ȃ����߁A���̃��[�v���ɓ���ƁA����������I�ɒǉ����܂��B
        if (numComponents == 0) 
            numComponents++;

        if (buffer[index] == ' ') 
            numComponents++;

        if (buffer[index] == '.') 
            decimalFound = true;

        if ((buffer[index] >= 'a' && buffer[index] <= 'z') ||
            (buffer[index] >= 'A' && buffer[index] <= 'Z') ||
            buffer[index] == '_') 
            charFound = true;

        index++;

    }

    // ���O�̌�ɕϐ���1�����\������Ă���ꍇ�Avector��3�ł���K�v�����邽�߁Avector�ȊO�̔C�ӂ̃^�C�v�ɂ��邱�Ƃ��ł��܂��B
    switch (numComponents)
    {
    case 1:
        // ����������ꍇ�A����͕�����ł��B
        if (charFound) 
            type = STRING_VAR;
        else 
            type = INT_VAR;

        // ����������A�������Ȃ��ꍇ�́Afloat�ɂȂ�܂��B
        if (decimalFound == true && charFound == false) 
            type = FLOAT_VAR;
        break;

    case 3:
        // �x�N�g���͒P�Ȃ�float�ł��邽�߁A�O���[�v���ɕ������܂܂�Ă���ꍇ�́A������ł���B
        if (charFound) 
            type = STRING_VAR;
        else 
            type = VECTOR_VAR;
        break;

    default:
        // ���O�̌�ɕ����̒P�ꂪ����ꍇ�A������ɂ̓X�y�[�X���܂߂邱�Ƃ��ł��邽�߁A������ł���B
        if (numComponents > 0) 
            type = STRING_VAR;
        break;
    }

    return type;
}


void CVariable::SetData(int t, char* n, void* data)
{
    if (!name) return;

    // �ϐ�����ݒ肵�Ă���A�^�C�v�ƃf�[�^��ݒ肵�܂��B
    memcpy(name, n, strlen(n));
    name[strlen(n)] = '\0';
    SetData(t, data);
}


void CVariable::SetData(int t, void* data)
{
    stVector* vec = NULL;
    int len = 0;

    // �^�C�v�ɉ����āA�l���i�[����Ă���ꏊ�ɂ���ĈقȂ�܂��B
    switch (t)
    {
    case INT_VAR:
        intVal = (int)data;
        break;

    case FLOAT_VAR:
        floatVal = *(float*)data;
        break;

    case STRING_VAR:
        len = strlen((char*)data);
        stringVal = new char[len + 1];
        memcpy(stringVal, (char*)data, len);
        stringVal[len] = '\0';
        break;

    case VECTOR_VAR:
        vec = (stVector*)data;
        vecVal.x = vec->x;
        vecVal.y = vec->y;
        vecVal.z = vec->z;
        break;

    default:
        // �����ɓ��B����ƁA�����NULL�ϐ��ł��B
        return;
        break;
    };

    type = t;
}


CPropertyScript::CPropertyScript() : variableList(NULL),
m_totalVars(0), currentLineChar(0)
{

}


CPropertyScript::~CPropertyScript()
{
    // ���ׂẴ��\�[�X��������܂��B
    Shutdown();
}


bool CPropertyScript::IncreaseVariableList()
{
    if (!variableList)
    {
        variableList = new CVariable[1];

        if (!variableList) 
            return false;
    }
    else
    {
        CVariable* temp;
        temp = new CVariable[m_totalVars + 1];
        if (!temp) 
            return false;

        memcpy(temp, variableList,
            sizeof(CVariable) * m_totalVars);

        delete[] variableList;
        variableList = temp;
    }

    return true;
}


bool CPropertyScript::LoadScriptFile(char* filename)
{
    int totalScriptLines = 0;
    ifstream input, input2;
    char tempLine[256];
    char varName[128];
    char param[3072];
    int type = 0;

    // �t�@�C�����J���āA��������s�����擾���܂��B
    input.open(filename);
    if (!input.is_open()) 
        return false;

    // �ȑO�̃f�[�^�����ׂăN���A���܂��B
    Shutdown();

    // �s�����擾���܂��B
    while (!input.eof())
    {
        input.getline(tempLine, 256, '\n');
        totalScriptLines++;
    }

    input.close();

    // ����͕ϐ������o���܂��B
    input2.open(filename);
    if (!input2.is_open()) 
        return false;

    // �X�N���v�g�̊e�s�����[�v���āA���ׂĂ̕ϐ����擾���܂��B
    for (int i = 0; i < totalScriptLines; i++)
    {
        // ���C���J�E���^�[���ŏ��Ƀ��Z�b�g���܂��B
        currentLineChar = 0;

        // �t�@�C������1�s��ǂݎ��܂��B
        input2.getline(tempLine, 256, '\n');
        tempLine[strlen(tempLine)] = '\0';

        // ���ꂪ�R�����g���ǂ������m�F����A�Ȃ��ꍇ�p�����܂��B
        if (tempLine[0] != '#')
        {
            // ���O��ǂ݁A�^�C�v�𔻕ʂ��܂��B
            ParseNext(tempLine, varName);
            type = DetermineType(currentLineChar, tempLine);

            // �^�C�v�ɂ���ẮA���O�̌�ɓǂޕK�v�̂���P��̐��ɂ���ĈقȂ�܂��B int�̏ꍇ��1�A�x�N�g��3�A������1�Ȃǂ��K�v�ł��B
            // �f�[�^���擾������A�����K�v�ȃ^�C�v�ɕϊ����A�ϐ��ɐݒ肵�܂��B
            switch (type)
            {
            case INT_VAR:
                if (IncreaseVariableList())
                {
                    ParseNext(tempLine, param);
                    variableList[m_totalVars].SetData(INT_VAR, varName, (void*)atoi(param));
                    m_totalVars++;
                }
                break;
            case FLOAT_VAR:
                if (IncreaseVariableList())
                {
                    float fVal = 0;
                    ParseNext(tempLine, param);
                    fVal = (float)atof(param);
                    variableList[m_totalVars].SetData(FLOAT_VAR, varName, &fVal);
                    m_totalVars++;
                }
                break;
            case STRING_VAR:
                if (IncreaseVariableList())
                {
                    ParseNext(tempLine, param);
                    variableList[m_totalVars].SetData(STRING_VAR, varName, (void*)param);
                    m_totalVars++;
                }
                break;
            case VECTOR_VAR:
                if (IncreaseVariableList())
                {
                    stVector vecVal;

                    ParseNext(tempLine, param);
                    vecVal.x = (float)atof(param);
                    ParseNext(tempLine, param);
                    vecVal.y = (float)atof(param);
                    ParseNext(tempLine, param);
                    vecVal.z = (float)atof(param);

                    variableList[m_totalVars].SetData(VECTOR_VAR, varName, &vecVal);
                    m_totalVars++;
                }
                break;
            default:
                break;
            }
        }
    }

    // �t�@�C������܂�.
    input2.close();
    return true;
}


void CPropertyScript::ParseNext(char* tempLine, char* outData)
{
    int commandSize = 0;
    int paramSize = 0;

    // �G���[�`�F�b�N���܂��B
    if (!tempLine || !outData) 
        return;

    // �z������������܂��B
    outData[0] = '\0';

    // �X�y�[�X�܂��͉��s��������܂ŁA���ׂĂ̕�����ǂݍ��݂܂��B
    while (currentLineChar < (int)strlen(tempLine))
    {
        if (tempLine[currentLineChar] == ' ' || tempLine[currentLineChar] == '\n')
            break;

        // �f�[�^��ۑ�
        outData[paramSize] = tempLine[currentLineChar];
        paramSize++;
        currentLineChar++;
    }

    // '\0'�o�͂��āA���̃X�y�[�X�܂ňړ����܂��B�X�y�[�X���Ȃ��ꍇ�A���̍s�Ɉړ����܂��B
    outData[paramSize] = '\0';
    currentLineChar++;
}


bool CPropertyScript::AddVariable(char* name, int t, void* val)
{
    // �ϐ������łɑ��݂��邩�ǂ������m�F���܂��B
    if (!SetVariable(name, t, val))
    {
        if (!IncreaseVariableList()) 
            return false;

        // �ϐ��f�[�^��ݒ肵�܂��B
        variableList[m_totalVars].SetData(t, name, val);
        m_totalVars++;
    }

    return true;
}


bool CPropertyScript::SetVariable(char* name, int t, void* val)
{
    // ���X�g�����[�v���Ė��O���r���܂��B�ϐ������������ꍇ�́A���̃f�[�^��ݒ肵�܂��B
    for (int i = 0; i < m_totalVars; i++)
    {
        if (_stricmp(variableList[i].GetName(), name) == 0)
        {
            variableList[i].SetData(t, val);
            return true;
        }
    }

    return false;
}


int CPropertyScript::GetVariableAsInt(char* name)
{
    // ���X�g�����[�v���Ė��O���r���܂��B�ϐ������������ꍇ�́A���̃f�[�^��ݒ肵�܂��B
    for (int i = 0; i < m_totalVars; i++)
    {
        if (_stricmp(variableList[i].GetName(), name) == 0)
            return variableList[i].GetDataAsInt();
    }

    return 0;
}


float CPropertyScript::GetVariableAsFloat(char* name)
{
    // ���X�g�����[�v���Ė��O���r���܂��B�ϐ������������ꍇ�́A���̃f�[�^��ݒ肵�܂��B
    for (int i = 0; i < m_totalVars; i++)
    {
        if (_stricmp(variableList[i].GetName(), name) == 0)
            return variableList[i].GetDataAsFloat();
    }

    return 0;
}


char* CPropertyScript::GetVariableAsString(char* name)
{
    // ���X�g�����[�v���Ė��O���r���܂��B�ϐ������������ꍇ�́A���̃f�[�^��ݒ肵�܂��B
    for (int i = 0; i < m_totalVars; i++)
    {
        if (_stricmp(variableList[i].GetName(), name) == 0)
            return variableList[i].GetDataAsString();
    }

    return NULL;
}


stVector CPropertyScript::GetVariableAsVector(char* name)
{
    stVector null;

    //���X�g�����[�v���āA���O���r���܂��B�ϐ������������ꍇ�A���̃f�[�^��Ԃ��܂��B
    for (int i = 0; i < m_totalVars; i++)
    {
        if (_stricmp(variableList[i].GetName(), name) == 0)
            return variableList[i].GetDataAsVector();
    }

    return null;
}


void CPropertyScript::Shutdown()
{
    // ���X�g�폜���܂��B
    if (variableList) 
        delete[] variableList;
    variableList = NULL;
}