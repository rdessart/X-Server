#pragma once

#include <map>
#include <string>
#include <vector>
#include <Datarefs/Dataref.h>

const int MaximumMPAircraft = 19;
const int MaximumAircrafts = 64;

struct Position 
{
	double Latitude;
	double Longitude;
	double Elevation;

	float Pitch;
	float Roll;
	float Heading;

	float Vx;
	float Vy;
	float Vz;

	//float Slats;
	//float Flaps;
	//float Flaps2;
	//float Spoiler;
	//float Speedbrakes;
	//float WingSweeps;
	//float LandingGear;
};

struct Aircraft 
{
	std::string AircraftModelPath;
	int aircraftId;
	Position position;
	bool isOverriden;
};

class AircraftManager
{
public:
	AircraftManager();

	bool AquireAircrafts();
	void ReleaseAircrafs();

	void SetPlaneCount(int count);

	void SetOverridePlanePath(int id, int value);

	int RegisterAircraft(std::string path);
	void UpdateAircraft(int id, Position position);
	void DeleteAircraft(int id);

protected:
	std::map<int, Aircraft> m_aircrafts;
	std::vector<std::map<std::string, Dataref*>> m_datarefs;

	bool RegisterDatarefs(int id, std::string name, std::string path, Dataref::Type type = Dataref::Type::Unknown);
	XPLMDataRef m_overridePlaneDataref;
};

