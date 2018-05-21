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
#endif
