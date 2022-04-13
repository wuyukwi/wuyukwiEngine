/*********************************************************************************

  *FileName: Script.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/02

**********************************************************************************/


#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#define MAX_COMMAND_SIZE   32
#define MAX_LINE_SIZE      3072
#define MAX_PARAM_SIZE     2048

#include "structs.h"

class CScript
{
   public:
      CScript();
      ~CScript();
      
      bool LoadScriptFile(const char *filename);
      void ParseCommand(char *destCommand);
      void ParseStringParam(char *destString);
      bool ParseBoolParam();
      int ParseIntParam();
      float ParseFloatParam();
      void ParseSoundFileParan(char* destString, int* repeats);
      void Shutdown();

      void MoveToStart();
      void MoveToNextLine();
      int GetCurrentLineNum();
      int GetTotalLines();
      bool IsLineComment();

      bool SaveScript(const char* filename, const char** data, int currentLine, int currentLineChar);

   private:
      int totalScriptLines;
      int currentLine;
      int currentLineChar;
      char **m_script;
};

#endif