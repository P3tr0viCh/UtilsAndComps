// ---------------------------------------------------------------------------

#ifndef UtilsCryptoPPH
#define UtilsCryptoPPH

#include <System.hpp>

// ---------------------------------------------------------------------------
String EncryptAES(String Text, String Key);
String DecryptAES(String Text, String Key);

// ---------------------------------------------------------------------------
String HashSHA256(String Text);

// ---------------------------------------------------------------------------
String CRC(String Text);
String Encrypt(String Text, String Key);
String Decrypt(String Text, String Key);
void CheckCRC(String CheckedStr, String EncryptedCRC, String Key);

// ---------------------------------------------------------------------------
#endif
