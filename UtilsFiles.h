// ---------------------------------------------------------------------------

#ifndef UtilsFilesH
#define UtilsFilesH
// ---------------------------------------------------------------------------

#include <System.Types.hpp>

bool EXEIsRunning(String FileName, bool FullPath);
bool OKExec(const int Inst, String FileName = NULL);
bool ShellExec(const String FileName);
HINSTANCE ShellExecEx(const String FileName, const String Params);

String FileInAppDir(String FileName);
String FileWithoutExt(String FileName);
String OnlyFileName(String FileName);
#endif
