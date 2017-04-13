// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsBase64.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------

char alfBase64[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

PACKAGE AnsiString __fastcall ToBase64(const void* databuf,
	unsigned int bufsize) {
	if (databuf == NULL)
		return "";
	if (bufsize == 0x00)
		return "";

	AnsiString rettext = "";

	int ibuf;
	char b64[5];
	char *cp = (char*)(databuf);

	for (unsigned int ui = 0; ui < bufsize;) {
		memset(b64, 0x00, sizeof(b64));
		ibuf = (*cp & 0xFF) << 16;
		ui++;
		cp++;
		if (ui < bufsize) {
			ibuf |= (*cp & 0xFF) << 8;
			ui++;
			cp++;
		}
		else
			b64[2] = b64[3] = '=';
		if (ui < bufsize) {
			ibuf |= (*cp & 0xFF) << 0;
			ui++;
			cp++;
		}
		else
			b64[3] = '=';

		b64[0] = alfBase64[(ibuf >> 18) & 0x3F];
		b64[1] = alfBase64[(ibuf >> 12) & 0x3F];
		if (b64[2] == 0)
			b64[2] = alfBase64[(ibuf >> 6) & 0x3F];
		if (b64[3] == 0)
			b64[3] = alfBase64[(ibuf >> 0) & 0x3F];
		rettext += AnsiString(b64);
	}

	return rettext;
}

PACKAGE unsigned long __fastcall FromBase64(const AnsiString& text,
	const void* databuf, unsigned int bufsize) {
	if (databuf == NULL)
		return 0l;
	if (bufsize == 0x00)
		return 0l;

	char *cpoi = (char*)databuf;
	unsigned long ret = 0l;
#define add_char(chr)   { *cpoi++ = chr; ret++; if (ret >= bufsize) return ret; }

	char strn[4];
	int shift = 18;
	int ibuf = 0;
	for (char *cp = text.c_str(); *cp; cp++) {
		if (*cp == '=')
			break;

		char *bp = (char*)(memchr(alfBase64, *cp, sizeof(alfBase64)));
		if (!bp)
			continue;
		ibuf |= ((bp - alfBase64) & 0x3F) << shift;
		shift -= 6;
		if (shift < 0) {
			add_char((ibuf >> 16) & 0xFF);
			add_char((ibuf >> 8) & 0xFF);
			add_char((ibuf >> 0) & 0xFF);
			shift = 18;
			ibuf = 0;
		}
	}

	if (shift < 12)
		add_char((ibuf >> 16) & 0xFF);
	if (shift < 6)
		add_char((ibuf >> 8) & 0xFF);
	if (shift < 0)
		add_char((ibuf >> 0) & 0xFF);

#undef add_chr;
	return ret;
}

PACKAGE AnsiString __fastcall ToBase64(const AnsiString& text) {
	return ToBase64(text.c_str(), text.Length());
}

PACKAGE AnsiString __fastcall FromBase64(const AnsiString& text) {
	int len = ((text.Length() + 3) / 4) * 3;
	char *buf = new char[len + 1];

	len = FromBase64(text, buf, len);

	buf[len] = 0x00;
	AnsiString rettext = AnsiString(buf);
	delete[]buf;
	return rettext;
}
