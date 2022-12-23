// ---------------------------------------------------------------------------

#pragma hdrstop

#include <Sysutils.hpp>

// add to Library path "..\CryptoPP"
#pragma link "CryptoPP.lib"

// add to Include path "..\CryptoPP\cryptopp700"
#include "default.h"
#include "hex.h"
#include "aes.h"
#include "sha.h"

#include "UtilsCryptoPP.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

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
String CRC(String Text) {
	try {
		return HashSHA256(Text);
	}
	catch (...) {
		return "";
	}
}

// ---------------------------------------------------------------------------
String Encrypt(String Text, String Key) {
	if (Text.IsEmpty()) {
		return "";
	}

	try {
		return EncryptAES(Text, Key);
	}
	catch (...) {
		throw EEncodingError("encrypt");
	}
}

// ---------------------------------------------------------------------------
String Decrypt(String Text, String Key) {
	if (Text.IsEmpty()) {
		return "";
	}

	try {
		return DecryptAES(Text, Key);
	}
	catch (...) {
		throw EEncodingError("decrypt");
	}
}

// ---------------------------------------------------------------------------
void CheckCRC(String CheckedStr, String EncryptedCRC, String Key) {
	if (EncryptedCRC.IsEmpty()) {
		throw EEncodingError("CRC empty");
	}

	String DecryptedCRC = Decrypt(EncryptedCRC, Key);

	String CheckedStrCRC = CRC(CheckedStr);

	if (!SameStr(CheckedStrCRC, DecryptedCRC)) {
		throw EEncodingError("CRC wrong");
	}
}

// ---------------------------------------------------------------------------
