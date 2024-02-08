#include "../../include/Datarefs/Dataref.h"

/* \brief Default Constructor */
Dataref::Dataref() :
	m_dataref(nullptr), m_type(Dataref::Type::Unknown),
	m_logger(Logger("X-Server.log", "Dataref", false)),
	m_link(""),
	m_conversionFactor(1.0)
{
	TypeDataref = DatarefType::XPLMDataref;
}

/* \brief Copy Constructor */
Dataref::Dataref(const Dataref& rhs)
{

	m_dataref = rhs.m_dataref;
	m_type = rhs.m_type;
}

Dataref::~Dataref()
{
}

/* \brief Load a dataref with a specific path
* \param[in] path the path of the dataref
* \return boolean True if the path return a valid dataref
*/
bool Dataref::Load(std::string path)
{
	m_link = path;
	m_dataref = XPLMFindDataRef(path.c_str());
	m_logger.Log("Loading Dataref '" + path + "'");
	if (m_dataref == nullptr)
	{
		m_logger.Log("Loading Dataref '" + path + "' : FAILED", Logger::Severity::WARNING);
		return false;
	}
	m_logger.Log("Loaded Dataref '" + path + "'");
	m_type = this->LoadType();
	m_logger.Log("Loaded Type '" + std::to_string((int)m_type) + "'");
	return true;
}

bool Dataref::CanWrite() const
{
	return m_dataref && XPLMCanWriteDataRef(m_dataref);
}

bool Dataref::IsGood() const
{
	return m_dataref && XPLMIsDataRefGood(m_dataref);
}

Dataref::Type Dataref::GetType() const
{
	return m_type;
}

Dataref::Type Dataref::LoadType()
{
	if (m_dataref != nullptr)
	{
		m_type = (Dataref::Type)XPLMGetDataRefTypes(m_dataref);
		// handling case where data can be either double or int
		//if ((int)m_type && ((int)Type::Float || (int)Type::Double))
		if ((int)m_type & ((int)Type::Float | (int)Type::Double))
		{
			m_type = Type::Double;
		}
		return m_type;
	}
	return Dataref::Type::Unknown;
}

void Dataref::SetType(Dataref::Type newType)
{
	m_type = newType;
}

void Dataref::SetType(std::string newType)
{
	if (StringToType.contains(newType))
	{
		m_type = StringToType.at(newType);
	}
	else {
		m_type = Dataref::Type::Unknown;
	}
}

json Dataref::GetValue() const
{
	if (m_dataref == NULL)
	{
		return json();
	}

	json value;
	switch (m_type)
	{
	case Dataref::Type::Unknown:
		break;
	case Dataref::Type::Int:
		value = XPLMGetDatai(m_dataref) * (int)m_conversionFactor;
		
	case Dataref::Type::Float:
		value = XPLMGetDataf(m_dataref) * (float)m_conversionFactor;
		break;
	case Dataref::Type::Double:
	{
		value = XPLMGetDatad(m_dataref) * m_conversionFactor;
		break;
	}
	case Dataref::Type::FloatArray:
	{
		int arraySize = XPLMGetDatavf(m_dataref, nullptr, 0, 0);
		float* floatArray = (float*)malloc(sizeof(float) * arraySize);
		if (floatArray == nullptr) return "";
		XPLMGetDatavf(m_dataref, floatArray, 0, arraySize);
		value = json::array();
		for (int i = 0; i < arraySize; i++)
		{
			float item = *((float*)(floatArray + i));
			value.push_back(item * (float)m_conversionFactor);
		}
		free(floatArray);
		return value;
	}
	case Dataref::Type::IntArray:
	{
		int arraySize = XPLMGetDatavi(m_dataref, nullptr, 0, 0);
		int* intArray = (int*)malloc(sizeof(int) * arraySize);
		if (intArray == nullptr) return "";
		XPLMGetDatavi(m_dataref, intArray, 0, arraySize);
		value = json::array();
		for (int i = 0; i < arraySize; i++)
		{
			int item = *((int*)(intArray + i));
			value.push_back(item * (int)m_conversionFactor);
		}
		free(intArray);
		break;
	}
	case Dataref::Type::Data:
	{
		int lenght = XPLMGetDatab(m_dataref, NULL, 0, 0);
		char* data = (char*)malloc(lenght * sizeof(char));
		if (data == nullptr) return "";
		memset(data, 0x00, lenght);
		XPLMGetDatab(m_dataref, (void*)data, 0, lenght);
		value = std::string(data).substr(0, strlen(data));
		m_logger.Log("[GET DATA]" + m_link + " = " + data);
		free(data);
		break;
	}
	default:
		break;
	}
	return value;
}

void Dataref::SetValue(json value) const
{
	if (m_dataref == NULL || !this->CanWrite())
	{
		m_logger.Log("Dataref is null", Logger::Severity::WARNING);
		return;
	}
	switch (m_type)
	{
	case Dataref::Type::Unknown:
		break;
	case Dataref::Type::Int:
	{
		m_logger.Log("Dataref is of type int", Logger::Severity::TRACE);
		int val = value.get<int>();
		val /= (int)m_conversionFactor;
		XPLMSetDatai(m_dataref, val);
		break;
	}
	case Dataref::Type::Float:
	{
		m_logger.Log("Dataref is of type float", Logger::Severity::TRACE);
		float val = value.get<float>() / (float)m_conversionFactor;
		XPLMSetDataf(m_dataref, val);
		break;
	}
	case Dataref::Type::Double:
	{
		m_logger.Log("Dataref is of type double", Logger::Severity::TRACE);
		double val = value.get<double>() / m_conversionFactor;
		m_logger.Log("Value: " + std::to_string(val), Logger::Severity::TRACE);
		XPLMSetDatad(m_dataref, val);
		break;
	}
	case Dataref::Type::FloatArray:
	{
		m_logger.Log("Dataref is of type floatarray", Logger::Severity::WARNING);
		int res = this->setFloatArrayFromJson(0, value);
		break;
	}
	case Dataref::Type::IntArray:
	{
		m_logger.Log("Dataref is of type intarray", Logger::Severity::WARNING);
		int res = this->setIntArrayFromJson(0, value);
		break;
	}
	case Dataref::Type::Data:
	{
		std::string dataString = value.get<std::string>();
		int maxLenght = XPLMGetDatab(m_dataref, NULL, 0, 0);
		char* zero = (char*)malloc(sizeof(char) * maxLenght);
		if (zero == nullptr) return;
		memset(zero, 0x00, maxLenght);
		XPLMSetDatab(m_dataref, zero, 0, maxLenght);
		free(zero);

		m_logger.Log("[SET DATA]" + m_link + " has a max size of" + std::to_string(maxLenght));
		std::size_t lenght = dataString.size();
		m_logger.Log("[SET DATA]" + dataString + " has a size of" + std::to_string(lenght));
		if (lenght > maxLenght)
		{
			lenght = maxLenght;
		}
		m_logger.Log("[SET DATA]" + m_link + " size of" + std::to_string(lenght));
		XPLMSetDatab(m_dataref, (void*)dataString.c_str(), 0, (int)lenght);
		m_logger.Log("[SET DATA]" + m_link + " = " + dataString);
		break;
	}
	default:
		break;
	}
}

void Dataref::SetConversionFactor(json conversionFactor)
{
	m_conversionFactor = conversionFactor.get<double>();
}

int Dataref::setFloatArrayFromJson(int offset, json value) const
{
	std::vector<float> data;
	int maxSize = XPLMGetDatavf(m_dataref, nullptr, 0, 0);
	int f_offset(offset);
	if (value.type() == json::value_t::discarded)
	{
		m_logger.Log("FloatArray : json parsing of value failed!", Logger::Severity::CRITICAL);
		return -1;
	}
	else if (value.type() == json::value_t::array)
	{
		if ((int)value.size() < maxSize)
			maxSize = (int)value.size();
		m_logger.Log("FloatArray : max size is " + std::to_string(maxSize), Logger::Severity::CRITICAL);
		std::vector<float> valArray = value.get<std::vector<float>>();
		for (auto& val : valArray)
		{
			data.push_back(val / (float)m_conversionFactor);
		}
	}
	else if (value.type() == json::value_t::string || value.type() == json::value_t::number_float)
	{
		if (value.type() == json::value_t::number_float)
		{
			for (int i(0); i < maxSize; i++)
			{
				data.push_back(value.get<float>() / (float)m_conversionFactor);
			}
		}
	}
	else if (value.type() == json::value_t::object)
	{
		if (!value.contains("Value"))
		{
			m_logger.Log("FloatArray : json is not an array and doesn't contain a Value field", Logger::Severity::CRITICAL);
			return -2;
		}
		if (value.contains("Offset"))
		{
			if (value["Offset"].type() == json::value_t::string)
			{
				f_offset = value["Offset"].get<int>() / (int)m_conversionFactor;
			}
			else
			{
				m_logger.Log("FloatArray : Offset field exist but is not string... skipping", Logger::Severity::WARNING);
				f_offset = 0;
			}
		}
		return setFloatArrayFromJson(f_offset, value["Value"].dump());
	}
	else {
		m_logger.Log("FloatArray: JSON type is unknown", Logger::Severity::CRITICAL);
		return 0x02;
	}
	data.resize(maxSize);
	XPLMSetDatavf(m_dataref, data.data(), offset, maxSize);
	return EXIT_SUCCESS;
}

int Dataref::setIntArrayFromJson(int offset, json value) const
{
	//TODO: COmplete function
	return 0;
}

void Dataref::FromJson(json data)
{
	m_link = data["Link"];
	m_conversionFactor = data.value("ConversionFactor", 1.0);
	this->SetType(data.value("Type", "Unknown"));
	m_dataref = XPLMFindDataRef(m_link.c_str());
	if(!this->IsGood())
	{
		m_logger.Log("Dataref: '" + m_link + "' is NOT Good()", Logger::Severity::CRITICAL);
	}
	if(!this->CanWrite())
	{
		m_logger.Log("Dataref: '" + m_link + "' is READONLY", Logger::Severity::WARNING);
	}
	if(!data.contains("Type"))
	{
		this->LoadType();
	}
	if(data.contains("Value"))
	{
		//check if type is string or int/float/json
		std::string value = ExtractValueJson(data["Value"]);
		this->SetValue(value);
	}
}

json Dataref::ToJson() const
{
	json j = json();
	j["Type"] = m_type;
	j["Link"] = m_link;
	j["ConversionFactor"] = m_conversionFactor;
	j["Good"] = this->IsGood();
	j["ReadOnly"] = !this->CanWrite();
	j["Value"] = this->GetValue();
	return j;
}
