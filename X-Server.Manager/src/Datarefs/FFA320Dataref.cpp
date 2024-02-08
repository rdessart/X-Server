#include "../../include/Datarefs/FFA320Dataref.h"

FFDataref::FFDataref() : m_id(-1),
m_type(Type::Deleted), m_link(""),
m_conversionFactor(1.0),
m_ffapi(nullptr),
m_logger(Logger("X-Server.log", "FFDataref", false)),
m_needUpdate(false)
{
	TypeDataref = DatarefType::FFDataref;
}

FFDataref::FFDataref(SharedValuesInterface* FF_A320_api) : FFDataref()
{
	BindAPI(FF_A320_api);
}

FFDataref::FFDataref(const FFDataref& rhs)
{
	m_ffapi = rhs.m_ffapi;
	m_id = rhs.m_id;
	m_conversionFactor = rhs.m_conversionFactor;
	m_link = rhs.m_link;
	m_type = rhs.m_type;
}

FFDataref::~FFDataref()
{}

bool FFDataref::Load(std::string path)
{
	m_link = path;
	m_id = m_ffapi->ValueIdByName(path.c_str());
	if (m_id < 0) return false;
	Type t = LoadType();
	return t != Type::Deleted;
}

FFDataref::Type FFDataref::GetType()
{
	return m_type;
}

FFDataref::Type FFDataref::LoadType()
{
	if (m_id < 0) {
		m_logger.Log(m_link + " : Id is less than 1");
		return Type::Deleted;
	}
	m_type = (Type)m_ffapi->ValueType(m_id);
	m_logger.Log(std::string("[" + m_link + "] = " + std::to_string((int)m_type) + "\n").c_str());
	return m_type;
}

std::string FFDataref::GetValue() const
{
	m_logger.Log("FFDATAREF GetValue");
	if (m_ffapi == nullptr) {
		m_logger.Log(m_link + ": FlightFactor API pointer is NULL!", Logger::Severity::WARNING);
		return "FFAPI NOT FOUND";
	};
	if (m_id < 0 && m_type == Type::Deleted)
	{
		if (m_id < 0) m_logger.Log(m_link + ": id is INVALID ID", Logger::Severity::WARNING);
		if (m_type == Type::Deleted) m_logger.Log(m_link + ": type is 'Deleted'", Logger::Severity::WARNING);
		return "ID OR TYPE INVALID";
	}
	double converstionfactor = m_conversionFactor;
	switch (m_type)
	{
	case FFDataref::Type::Char:
	{
		char data(0);
		m_ffapi->ValueGet(m_id, &data);
		return std::to_string(data * (int)converstionfactor);
	}
	case FFDataref::Type::uChar:
	{
		unsigned char data(0);
		m_ffapi->ValueGet(m_id, &data);
		return std::to_string(data * (int)converstionfactor);
	}
	case FFDataref::Type::Short:
	{
		short data(0);
		m_ffapi->ValueGet(m_id, &data);
		return std::to_string(data * (int)converstionfactor);
	}
	case FFDataref::Type::uShort:
	{
		unsigned short data(0);
		m_ffapi->ValueGet(m_id, &data);
		return std::to_string(data * (int)converstionfactor);
	}
	case FFDataref::Type::Int:
	{
		int data(0);
		m_ffapi->ValueGet(m_id, &data);
		return std::to_string(data * (int)converstionfactor);
	}
	case FFDataref::Type::uInt:
	{
		unsigned int data(0);
		m_ffapi->ValueGet(m_id, &data);
		return std::to_string(data * (int)converstionfactor);
	}
	case FFDataref::Type::Float:
	{
		float data(0.0f);
		m_ffapi->ValueGet(m_id, &data);
		return std::to_string(data * (float)converstionfactor);
	}
	case FFDataref::Type::Double:
	{
		double data(0.0);
		m_ffapi->ValueGet(m_id, &data);
		return std::to_string(data * (double)converstionfactor);
	}
	case FFDataref::Type::String:
	{
		int lenght = m_ffapi->ValueGetSize(m_id);
		if (lenght <= 0) return "";
		void* buffer = malloc(sizeof(char));
		m_ffapi->ValueGet(m_id, buffer);
		auto str = (char*)malloc(lenght + 1);
		memset(str, 0x00, static_cast<size_t>(lenght+ 1));
#ifdef IBM
		memcpy_s(str, lenght, buffer, lenght);
#else
		memcpy(str, buffer, lenght);
#endif
		m_logger.Log(str);
		std::string value(str);
		return value;
	}
	default:
		m_logger.Log(m_link + ": has type " + std::to_string((int)m_type));
		return "N/A";
	}
}

//void FFDataref::SetValue(std::string value)
//{
//	m_targetValue = value;
//	m_needUpdate = true;
//}

// void FFDataref::SetValue(std::string value) const
// {
// 	m_logger.Log("[DO_SET_VALUE]" + m_link + " Setting value to " + value);
// 	int type = m_ffapi->ValueType(m_id);
// 	if (type == Value_Type_float32)
// 	{
// 		m_logger.Log("[DO_SET_VALUE]" + m_link + " Type is  " + std::to_string(type) + " (float32)");
// 		float val(0.0f);
// 		try
// 		{
// 			val = std::stof(value);
// 		}
// 		catch (std::invalid_argument)
// 		{
// 			m_logger.Log("[DO_SET_VALUE]" + m_link + " std::stof has crashed because of an invalid argument", Logger::Severity::CRITICAL);
// 			return;
// 		}
// 		m_ffapi->ValueSet(m_id, &val);
// 	}
// 	else if (type == Value_Type_float64)
// 	{
// 		m_logger.Log("[DO_SET_VALUE]" + m_link + " Type is  " + std::to_string(type) + " (float64)");
// 		double val(0.0);
// 		try
// 		{
// 			val = std::stod(value);
// 		}
// 		catch (std::invalid_argument)
// 		{
// 			m_logger.Log("[DO_SET_VALUE]" + m_link + " std::stof has crashed because of an invalid argument", Logger::Severity::CRITICAL);
// 			return;
// 		}
// 		m_ffapi->ValueSet(m_id, &val);
// 	}
// 	else if (type == Value_Type_sint32)
// 	{
// 		m_logger.Log("[DO_SET_VALUE]" + m_link + " Type is  " + std::to_string(type) + " (sint32)");
// 		int val(0);
// 		try
// 		{
// 			val = std::stoi(value);
// 		}
// 		catch (std::invalid_argument)
// 		{
// 			m_logger.Log("[DO_SET_VALUE]" + m_link + " std::stof has crashed because of an invalid argument", Logger::Severity::CRITICAL);
// 			return;
// 		}
// 		m_ffapi->ValueSet(m_id, &val);
// 	}
// }

void FFDataref::SetValue(json value) const
{
	m_logger.Log("[DO_SET_VALUE]" + m_link + " Setting value to " + value.get<std::string>());
	int type = m_ffapi->ValueType(m_id);
	if (type == Value_Type_float32)
	{
		m_logger.Log("[DO_SET_VALUE]" + m_link + " Type is  " + std::to_string(type) + " (float32)");
		float val(0.0f);
		try
		{
			val = value.get<float>();
		}
		catch (std::invalid_argument)
		{
			m_logger.Log("[DO_SET_VALUE]" + m_link + " std::stof has crashed because of an invalid argument", Logger::Severity::CRITICAL);
			return;
		}
		m_ffapi->ValueSet(m_id, &val);
	}
	else if (type == Value_Type_float64)
	{
		m_logger.Log("[DO_SET_VALUE]" + m_link + " Type is  " + std::to_string(type) + " (float64)");
		double val(0.0);
		try
		{
			val = value.get<double>();
		}
		catch (std::invalid_argument)
		{
			m_logger.Log("[DO_SET_VALUE]" + m_link + " std::stof has crashed because of an invalid argument", Logger::Severity::CRITICAL);
			return;
		}
		m_ffapi->ValueSet(m_id, &val);
	}
	else if (type == Value_Type_sint32)
	{
		m_logger.Log("[DO_SET_VALUE]" + m_link + " Type is  " + std::to_string(type) + " (sint32)");
		int val(0);
		try
		{
			val = value.get<int>();
		}
		catch (std::invalid_argument)
		{
			m_logger.Log("[DO_SET_VALUE]" + m_link + " std::stof has crashed because of an invalid argument", Logger::Severity::CRITICAL);
			return;
		}
		m_ffapi->ValueSet(m_id, &val);
	}
}

std::string FFDataref::GetTargetValue()
{
	return m_targetValue;
}

bool FFDataref::NeedUpdate() const
{
	return m_needUpdate;
}

XPLMDataRef FFDataref::GetNativeDataref()
{
	return nullptr;
}

void FFDataref::FromJson(json data)
{
	bool loaded = this->Load(data["Link"]);
	m_logger.Log("FFDataref Type is " + std::to_string(static_cast<int>(this->LoadType())));
	if (data.contains("Value"))
	{
		std::string value = ExtractValueJson(data["Value"]);
		this->SetValue(value);
	}
}

json FFDataref::ToJson() const
{
	json j = json();
	j["Type"] = m_type;
	j["FFId"] = m_id;
	j["FFName"] = GetName();
	j["FFDescription"] = GetDescription();
	j["FFFlag"] = GetFlag();
	j["FFUnit"] = GetUnit();
	j["Link"] = m_link;
	j["ConversionFactor"] = m_conversionFactor;
	j["Value"] = GetValue();
	return j;
}

void FFDataref::BindAPI(SharedValuesInterface* FF_A320_api)
{
	if (FF_A320_api != nullptr)
	{
		m_ffapi = FF_A320_api;
	}
}

void FFDataref::SetConversionFactor(json conversionFactor)
{
	m_conversionFactor = conversionFactor.get<double>();
}

int FFDataref::GetID() const
{
	return m_id;
}

int FFDataref::GetFlag() const
{
	if (m_id < 0) return -1;
	return m_ffapi->ValueType(m_id);
}

int FFDataref::GetUnit() const
{
	if (m_id < 0) return -1;
	return m_ffapi->ValueUnits(m_id);
}

std::string FFDataref::GetName() const
{
	if (m_id < 0) return "";
	return std::string(m_ffapi->ValueName(m_id));
}

std::string FFDataref::GetDescription() const
{
	if (m_id < 0) return "";
	return std::string(m_ffapi->ValueDesc(m_id));
}