#include "AircraftManager.h"
#include <XPLM/XPLMPlanes.h>
#include <XPLM/XPLMGraphics.h>

AircraftManager::AircraftManager()
{
}

bool AircraftManager::AquireAircrafts()
{
	int res = XPLMAcquirePlanes(nullptr, nullptr, nullptr);
	if (!res) return res;
	for (int i(1); i <= 19; i++)
	{
		RegisterDatarefs("PLAYER_" + std::to_string(i) + "_X", "sim/multiplayer/position/plane" + std::to_string(i) + "_x");
		RegisterDatarefs("PLAYER_" + std::to_string(i) + "_Y", "sim/multiplayer/position/plane" + std::to_string(i) + "_y");
		RegisterDatarefs("PLAYER_" + std::to_string(i) + "_Z", "sim/multiplayer/position/plane" + std::to_string(i) + "_z");
		RegisterDatarefs("PLAYER_" + std::to_string(i) + "_PITCH", "sim/multiplayer/position/plane" + std::to_string(i) + "_the");
		RegisterDatarefs("PLAYER_" + std::to_string(i) + "_ROLL", "sim/multiplayer/position/plane" + std::to_string(i) + "_phi");
		RegisterDatarefs("PLAYER_" + std::to_string(i) + "_HDG", "sim/multiplayer/position/plane" + std::to_string(i) + "_psi");
	}
}

void AircraftManager::ReleaseAircrafs()
{
	XPLMReleasePlanes();
	while (m_datarefs.size() > 0)
	{
		auto it = m_datarefs.begin();
		delete it->second;
		m_datarefs.erase(it);
	}
}

void AircraftManager::SetPlaneCount(int count)
{
	XPLMSetActiveAircraftCount(count);
}

int AircraftManager::RegisterAircraft(std::string path)
{
	int id = 0;
	for (int i(1); i < MaximumAircrafts; i++)
	{
		if (!m_aircrafts.contains(i))
		{
			id = i;
			break;
		}
	}
	Aircraft acf;
	acf.aircraftId = id;
	acf.AircraftModelPath = path;
	m_aircrafts.emplace(id, acf);
	if (id <= 19)
	{
		XPLMDisableAIForPlane(id);
		XPLMSetAircraftModel(id, path.c_str());
	}
	return id;
}

void AircraftManager::UpdateAircraft(int id, Position position)
{
	if (!m_aircrafts.contains(id)) return;
	if (id < MaximumMPAircraft)
	{
		double oX(0.0), oY(0.0), oZ(0.0);
		XPLMWorldToLocal(position.Latitude, position.Longitude, position.Elevation, &oX, &oY, &oZ);
		m_datarefs["PLAYER_" + std::to_string(id) + "_X"]->SetValue(oX);
		m_datarefs["PLAYER_" + std::to_string(id) + "_Y"]->SetValue(oY);
		m_datarefs["PLAYER_" + std::to_string(id) + "_Z"]->SetValue(oZ);
		m_datarefs["PLAYER_" + std::to_string(id) + "_PITCH"]->SetValue(position.Pitch);
		m_datarefs["PLAYER_" + std::to_string(id) + "_ROLL"]->SetValue(position.Roll);
		m_datarefs["PLAYER_" + std::to_string(id) + "_HDG"]->SetValue(position.Heading);
	}
}

void AircraftManager::DeleteAircraft(int id)
{
	m_aircrafts.erase(id);
}

bool AircraftManager::RegisterDatarefs(std::string name, std::string path)
{
	Dataref* dataref = new Dataref();
	bool res = dataref->Load(path);
	if (!res)
	{
		return res;
	}
	m_datarefs.emplace(name, dataref);
	return res;
}
