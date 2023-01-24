// ---------------------------------------------------------------------------

#ifndef UtilsFilesH
#define UtilsFilesH

#include <System.Types.hpp>

// ---------------------------------------------------------------------------
bool OKExec(const String FileName, const String Params = NULL);
void ShellExec(const String FileName, const String Params = NULL);

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
