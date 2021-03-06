/*
   Demo Name:  Game Project 13
      Author:  Allen Sherrod
     Chapter:  Chapter 15
*/


#include"LogSystem.h"


bool CLogSystem::ClearLog(const char *filename)
{
   errorLog.open(filename, std::ios::out);

   if(!Write(errorLog, ""))
      return false;
   
   errorLog.close();

   return true;
}


bool CLogSystem::WriteLog(const char *filename, const char *message, ...)
{
   va_list args;
   va_start(args, message);
   
   errorLog.open(filename, std::ios::out);

   if(!Write(errorLog, message, args)) return false;
   
   errorLog.close();
   return true;
}


bool CLogSystem::AppendLog(const char *filename, const char *message, ...)
{
   va_list args;
   va_start(args, message);
   
   errorLog.open(filename, std::ios::app);

   if(!Write(errorLog, message, args)) return false;

   errorLog.close();
   return true;
}


bool CLogSystem::Write(std::ofstream &file, const char *message)
{
   file << message;
   #ifdef DEBUG
      file.flush();
   #endif

   return true;
}


bool CLogSystem::Write(std::ofstream &file, const char *message, va_list &args)
{
   char buffer[1024];
   
   if(!file.is_open()) return false;
   
   vsprintf(buffer, message, args);

   file << buffer;
   #ifdef DEBUG
      file.flush();
   #endif

   return true;
}