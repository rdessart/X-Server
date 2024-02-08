#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class DatarefType {
    Abstract = 0,
    XPLMDataref = 1,
    FFDataref = 2,
};

class AbstractDataref {
public:
    DatarefType TypeDataref = DatarefType::Abstract;
    virtual json GetValue() const = 0;
    virtual void SetValue(json value) const = 0;
    virtual void SetConversionFactor(json conversionFactor) = 0;
    virtual bool Load(std::string path) = 0;
    virtual void FromJson(json data) = 0;
    virtual json ToJson() const = 0;
protected:
    std::string m_link;
    std::string m_conversionFactor;
};