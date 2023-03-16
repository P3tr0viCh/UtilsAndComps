// ---------------------------------------------------------------------------

#pragma hdrstop

#include <System.Math.hpp>

#include "UtilsGeo.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
bool IsGPXPointEqual(const TGPXPointRad Point1, const TGPXPointRad Point2) {
	return Point1.Latitude == Point2.Latitude && Point1.Longitude ==
		Point2.Longitude;
}

// ---------------------------------------------------------------------------
TGPXPointRad DegToRad(const TGPXPointDeg Point) {
	TGPXPointRad Result = {DegToRad(Point.Latitude), DegToRad(Point.Longitude)};
	return Result;
}

// ---------------------------------------------------------------------------
Extended Haversine(TGPXPointRad Point1, TGPXPointRad Point2) {
	if (IsGPXPointEqual(Point1, Point2)) {
		return 0.0;
	}

	Extended DeltaLatitude = Point2.Latitude - Point1.Latitude;
	Extended DeltaLongitude = Point2.Longitude - Point1.Longitude;

	Extended SinDeltaLatitude = Sin(DeltaLatitude / 2.0);
	Extended SinDeltaLongitude = Sin(DeltaLongitude / 2.0);

	Extended A = SinDeltaLatitude * SinDeltaLatitude + Cos(Point1.Latitude)
		* Cos(Point2.Latitude) * SinDeltaLongitude * SinDeltaLongitude;

	Extended C = 2.0 * atan2l(Sqrt(A), Sqrt(1 - A));

	return C * EARTH_RADIUS;
}

// ---------------------------------------------------------------------------
Extended Equirectangular(TGPXPointRad Point1, TGPXPointRad Point2) {
	if (IsGPXPointEqual(Point1, Point2)) {
		return 0.0;
	}

	Extended X = (Point2.Longitude - Point1.Longitude) * Cos
		((Point1.Latitude + Point2.Latitude) / 2);
	Extended Y = Point1.Latitude - Point2.Latitude;

	return Sqrt(X * X + Y * Y) * EARTH_RADIUS;
}

// ---------------------------------------------------------------------------
Extended SphericalLawOfCosines(TGPXPointRad Point1, TGPXPointRad Point2) {
	if (IsGPXPointEqual(Point1, Point2)) {
		return 0.0;
	}

	Extended SinLatitude1;
	Extended CosLatitude1;
	Extended SinLatitude2;
	Extended CosLatitude2;

	SinCos(Point1.Latitude, SinLatitude1, CosLatitude1);
	SinCos(Point2.Latitude, SinLatitude2, CosLatitude2);

	Extended SinLatitude = SinLatitude1 * SinLatitude2;
	Extended CosLatitude = CosLatitude1 * CosLatitude2;

	Extended CosLongitude = Cos(Point2.Longitude - Point1.Longitude);

	return ArcCos(SinLatitude + CosLatitude * CosLongitude) * EARTH_RADIUS;
}

// ---------------------------------------------------------------------------
