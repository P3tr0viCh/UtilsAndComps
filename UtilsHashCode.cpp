// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsHashCode.h"

#include <boost/functional/hash.hpp>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
std::size_t HashCode(String S) {
	boost::hash<std::string>StringHash;

	AnsiString AS(S);

	std::string SS(AS.c_str());

	return StringHash(SS);
}

// ---------------------------------------------------------------------------
