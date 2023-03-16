// ---------------------------------------------------------------------------

#ifndef UtilsGeoH
#define UtilsGeoH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#define EARTH_RADIUS 6372795

// ---------------------------------------------------------------------------
struct TGPXPointDeg {
	Extended Latitude;
	Extended Longitude;
};

// ---------------------------------------------------------------------------
struct TGPXPointRad {
	Extended Latitude;
	Extended Longitude;
};

// ---------------------------------------------------------------------------
bool IsGPXPointEqual(const TGPXPointRad Point1, const TGPXPointRad Point2);

// ---------------------------------------------------------------------------
TGPXPointRad DegToRad(const TGPXPointDeg Point);

// ---------------------------------------------------------------------------
Extended Haversine(TGPXPointRad Point1, TGPXPointRad Point2);
Extended Equirectangular(TGPXPointRad Point1, TGPXPointRad Point2);
Extended SphericalLawOfCosines(TGPXPointRad Point1, TGPXPointRad Point2);

// ---------------------------------------------------------------------------
#endif
