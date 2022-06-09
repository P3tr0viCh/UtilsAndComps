// ---------------------------------------------------------------------------

#ifndef AboutFrmH
#define AboutFrmH

#include <System.hpp>
#include <Vcl.Graphics.hpp>

// ---------------------------------------------------------------------------
void ShowAbout(int iFontSize = 26, Byte bLineBreak = MAXBYTE, Byte bDatePos =
	MAXBYTE, Byte bVersionPos = MAXBYTE, String sDate = NULL,
	TPicture * pPicture = NULL, String sAddComp = NULL, String sVersion = NULL,
	String sAppName = NULL, String sCopyright = NULL, String sText = NULL,
	HICON hIcon = NULL);

// ---------------------------------------------------------------------------
#endif
