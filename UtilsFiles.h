// ---------------------------------------------------------------------------

#ifndef UtilsFilesH
#define UtilsFilesH

#include <System.Types.hpp>

// ---------------------------------------------------------------------------
bool OKExec(const int Inst, String FileName = NULL);
bool ShellExec(const String FileName);
HINSTANCE ShellExecEx(const String FileName, const String Params);

// ---------------------------------------------------------------------------
String FileInAppDir(String FileName);
String FileWithoutExt(String FileName);
String OnlyFileName(String FileName);
String SlashSep(String Path, String S);

// ---------------------------------------------------------------------------
String GetSpecialFolderPath(int Csidl);
String GetTempFolderPath();
String GetTempFileNamePath(String Prefix);

// ---------------------------------------------------------------------------
#endif
