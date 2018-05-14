// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsCryptoPP.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "CryptoPP.lib"

#include "default.h"
#include "hex.h"
#include "aes.h"

// ---------------------------------------------------------------------------
String EncryptAES(String Text, String Key) {
	CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption e((byte*)Key.c_str(),
		Key.Length());

	std::string Result;

	CryptoPP::StringSource encryptor(AnsiString(Text).c_str(), true,
		new CryptoPP::StreamTransformationFilter(e,
		new CryptoPP::HexEncoder(new CryptoPP::StringSink(Result))));

	return String(Result.c_str());
}

// ---------------------------------------------------------------------------
String DecryptAES(String Text, String Key) {
	CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption d((byte*)Key.c_str(),
		Key.Length());

	std::string Result;

	CryptoPP::StringSource decryptor(AnsiString(Text).c_str(), true,
		new CryptoPP::HexDecoder(new CryptoPP::StreamTransformationFilter(d,
		new CryptoPP::StringSink(Result))));

	return String(Result.c_str());
}

// ---------------------------------------------------------------------------
