#pragma once
#include <XPLM/XPLMDataAccess.h>

#include "AbstractDataref.h"

#include "../Tools/SharedValue.h"
#include "../Tools/Utils.h"

#include <Logger.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class FFDataref :
    public AbstractDataref
{
public:
	enum class Type {
		Deleted = Value_Type_Deleted,
		Object = Value_Type_Object,
		Char = Value_Type_sint8,
		uChar = Value_Type_uint8,
		Short = Value_Type_sint16,
		uShort = Value_Type_uint16,
		Int = Value_Type_sint32,
		uInt = Value_Type_uint32,
		Float = Value_Type_float32,
		Double = Value_Type_float64,
		String = Value_Type_String,
		Time = Value_Type_Time,
	};
	FFDataref();
	FFDataref(SharedValuesInterface* FF_A320_api);
	FFDataref(const FFDataref& rhs);
	~FFDataref();
	bool Load(std::string path);
	FFDataref::Type GetType();
	FFDataref::Type LoadType();
	json GetValue() const;
	void SetValue(json value, int offset = 0) const;
	void BindAPI(SharedValuesInterface* FF_A320_api);
	void SetConversionFactor(json conversionFactor);
	std::string GetTargetValue();
	int GetID() const;
	int GetFlag() const;
	int GetUnit() const;
	std::string GetName() const;
	std::string GetDescription() const;
	bool NeedUpdate() const;
	XPLMDataRef GetNativeDataref();
	void FromJson(json data);
	json ToJson() const;
protected:
	bool m_needUpdate;
	std::string m_targetValue;
	std::string m_link;
	int m_id;
	Type m_type;
	double m_conversionFactor;
	SharedValuesInterface* m_ffapi;
	Logger m_logger;
	//void* m_buffer;
};

static std::map<std::string, FFDataref::Type> const FFStringToType{
	{"Unknow", FFDataref::Type::Deleted},
	{"Object", FFDataref::Type::Object},
	{"Char", FFDataref::Type::Char},
	{"uChar", FFDataref::Type::uChar},
	{"Short", FFDataref::Type::Short},
	{"uShort", FFDataref::Type::uShort},
	{"Int", FFDataref::Type::Int},
	{"uInt", FFDataref::Type::uInt},
	{"Float", FFDataref::Type::Float},
	{"Double", FFDataref::Type::Double},
	{"String", FFDataref::Type::String},
	{"Time", FFDataref::Type::Time},
};

