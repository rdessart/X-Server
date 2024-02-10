#pragma once

#include <map>
#include <string>
#include <Datarefs/Dataref.h>

const int MaximumMPAircraft = 19;
const int MaximumAircrafts = 64;

struct Position 
{
	double Latitude;
	double Longitude;
	double Elevation;
	double Pitch;
	double Roll;
	double Heading;

	float Slats;
	float Flaps;
	float Flaps2;
	float Spoiler;
	float Speedbrakes;
	float WingSweeps;
	float LandingGear;
};

struct Aircraft 
{
	std::string AircraftModelPath;
	int aircraftId;
	Position position;
};

class AircraftManager
{
public:
	AircraftManager();

	bool AquireAircrafts();
	void ReleaseAircrafs();

	void SetPlaneCount(int count);

	int RegisterAircraft(std::string path);
	void UpdateAircraft(int id, Position position);
	void DeleteAircraft(int id);

protected:
	std::map<int, Aircraft> m_aircrafts;
	std::map<std::string, Dataref*> m_datarefs;

	bool RegisterDatarefs(std::string name, std::string path);
};

