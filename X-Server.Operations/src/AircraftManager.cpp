#include "AircraftManager.h"
#include <XPLM/XPLMPlanes.h>
#include <XPLM/XPLMGraphics.h>

AircraftManager::AircraftManager()
{
	m_overridePlaneDataref = XPLMFindDataRef("sim/operation/override/override_planepath");
}

bool AircraftManager::AquireAircrafts()
{
	int res = XPLMAcquirePlanes(nullptr, nullptr, nullptr);
	if (!res) return res;
	std::string path_x   = "sim/multiplayer/position/planeX_x";
	std::string path_y   = "sim/multiplayer/position/planeX_y";
	std::string path_z   = "sim/multiplayer/position/planeX_z";
	std::string path_the = "sim/multiplayer/position/planeX_the";
	std::string path_phi = "sim/multiplayer/position/planeX_phi";
	std::string path_psi = "sim/multiplayer/position/planeX_psi";
	std::string path_vx = "sim/multiplayer/position/planeX_v_x";
	std::string path_vy = "sim/multiplayer/position/planeX_v_y";
	std::string path_vz = "sim/multiplayer/position/planeX_v_z";

	for (int i(1); i <= 19; i++)
	{
		path_x[30] = static_cast<char>(i + 0x30);
		path_y[30] = static_cast<char>(i + 0x30);
		path_z[30] = static_cast<char>(i + 0x30);
		path_vx[30] = static_cast<char>(i + 0x30);
		path_vy[30] = static_cast<char>(i + 0x30);
		path_vz[30] = static_cast<char>(i + 0x30);
		path_the[30] = static_cast<char>(i + 0x30);
		path_phi[30] = static_cast<char>(i + 0x30);
		path_psi[30] = static_cast<char>(i + 0x30);

		RegisterDatarefs(i, "PLAYER_X", path_x);
		RegisterDatarefs(i, "PLAYER_Y", path_y);
		RegisterDatarefs(i, "PLAYER_Z", path_z);
		RegisterDatarefs(i, "PLAYER_VX", path_vx, Dataref::Type::Float);
		RegisterDatarefs(i, "PLAYER_VY", path_vy, Dataref::Type::Float);
		RegisterDatarefs(i, "PLAYER_VZ", path_vz, Dataref::Type::Float);
		RegisterDatarefs(i, "PLAYER_PITCH", path_the, Dataref::Type::Float);
		RegisterDatarefs(i, "PLAYER_ROLL", path_phi,  Dataref::Type::Float);
		RegisterDatarefs(i, "PLAYER_HEADING", path_psi, Dataref::Type::Float);
	}
	return res;
}

void AircraftManager::ReleaseAircrafs()
{
	XPLMReleasePlanes();
	std::vector<int> planesOverride;
	for (int i(0); i < 19; i++)
	{
		planesOverride.push_back(0);
	}

	XPLMSetDatavi(m_overridePlaneDataref, &planesOverride[0], 1, static_cast<int>(planesOverride.size()));

	while (m_datarefs.size() > 0)
	{
		auto it = m_datarefs.begin();
		while (it->size() > 0)
		{
			auto it2 = it->begin();
			delete it2->second;
			it->erase(it2);
		}
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
	acf.isOverriden = false;
	m_aircrafts.emplace(id, acf);
	if (id <= 19)
	{
		XPLMDisableAIForPlane(id);
		XPLMSetAircraftModel(id, path.c_str());
	}
	return id;
}

void AircraftManager::SetOverridePlanePath(int id, int value)
{
	XPLMSetDatavi(m_overridePlaneDataref, &value, id, 1);
}

void AircraftManager::UpdateAircraft(int id, Position position)
{
	if (!m_aircrafts.contains(id)) return;
	m_aircrafts[id].position = position;
	if (id < MaximumMPAircraft)
	{
		double oX(0.0), oY(0.0), oZ(0.0);
		XPLMWorldToLocal(position.Latitude, position.Longitude, position.Elevation, &oX, &oY, &oZ);
		m_datarefs[id]["PLAYER_X"]->SetValue(oX);
		m_datarefs[id]["PLAYER_Y"]->SetValue(oY);
		m_datarefs[id]["PLAYER_Z"]->SetValue(oZ);
		//
		m_datarefs[id]["PLAYER_PITCH"]->SetValue(position.Pitch);
		m_datarefs[id]["PLAYER_ROLL"]->SetValue(position.Roll);
		m_datarefs[id]["PLAYER_HEADING"]->SetValue(position.Heading);

		m_datarefs[id]["PLAYER_VX"]->SetValue(position.Vx);
		m_datarefs[id]["PLAYER_VY"]->SetValue(position.Vy);
		m_datarefs[id]["PLAYER_VZ"]->SetValue(position.Vz);
	}
}

void AircraftManager::DeleteAircraft(int id)
{
	int val(0);
	XPLMSetDatavi(m_overridePlaneDataref, &val, id, 1);
	m_aircrafts.erase(id);
}

bool AircraftManager::RegisterDatarefs(int id, std::string name, std::string path, Dataref::Type type)
{
	while (id >= m_datarefs.size())
	{
		m_datarefs.push_back(std::map<std::string, Dataref*>());
	}
	Dataref* dataref = new Dataref();
	bool res = dataref->Load(path);
	if (!res)
	{
		return res;
	}
	if (type != Dataref::Type::Unknown)
	{
		dataref->SetType(type);
	}
	m_datarefs[id].emplace(name, dataref);
	return res;
}
