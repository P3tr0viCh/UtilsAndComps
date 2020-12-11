// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsCryptoPP.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// add to Library path "..\CryptoPP"
#pragma link "CryptoPP.lib"

// add to Include path "..\CryptoPP\cryptopp700"
#include "default.h"
#include "hex.h"
#include "aes.h"
#include "sha.h"

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
String HashSHA256(String Text) {
	CryptoPP::SHA256 Hash;
	std::string Result;

	CryptoPP::StringSource s(AnsiString(Text).c_str(), true,
		new CryptoPP::HashFilter(Hash,
		new CryptoPP::HexEncoder(new CryptoPP::StringSink(Result))));

	return String(Result.c_str());
}

// ---------------------------------------------------------------------------
