#include "Config.h"

#include <iostream>
#include <sstream>
#include <pugixml/pugixml.hpp>

#include "StringUtilities.h"

using pugi::xml_parse_result;
using pugi::xml_node;
using pugi::xml_node;

namespace Catalyst::Utilities
{
	Config::Config(char* _name)
		: m_doc{ new xml_document }, m_fileName{ _name }
	{
	}

	Config::~Config()
	{
		delete m_doc;
		m_doc = nullptr;
	}

	void Config::Load()
	{
		if (!Initialise())
			return;

		for (const xml_node& category : m_doc->child("Config").child("Categories").children("Category"))
		{
			string categoryTitle = category.attribute("title").as_string();

			for (const xml_node& value : category.children("Value"))
			{
				string valName = value.attribute("name").as_string();
				string valType = value.attribute("type").as_string();

				switch (StringToType(valType))
				{
				case EValType::Int:
					m_ints[categoryTitle][valName] = value.attribute("value").as_int();
					break;

				case EValType::Float:
					m_floats[categoryTitle][valName] = value.attribute("value").as_float();
					break;

				case EValType::Bool:
					m_bools[categoryTitle][valName] = value.attribute("value").as_bool();
					break;

				case EValType::String:
					m_strings[categoryTitle][valName] = value.attribute("value").as_string();
					break;

				case EValType::Vector:
					HandleVector(categoryTitle, valName, value.attribute("value").as_string());
					break;

				case EValType::Color:
					HandleColor(categoryTitle, valName, value.attribute("value").as_string());
					break;

				case EValType::Invalid:
					std::cout << "Type " << valType << " was invalid! Make sure you mark the type as one of the following: int, float, bool, string, vector, color\n";
					break;
				}
			}
		}
	}

	int Config::GetInt(const string& _category, const string& _name)
	{
		if(m_ints.contains(_category))
		{
			auto category = m_ints[_category];
			if(category.contains(_name))
				return category[_name];
		}

		return INT_MAX;
	}

	float Config::GetFloat(const string& _category, const string& _name)
	{
		if(m_floats.contains(_category))
		{
			auto category = m_floats[_category];
			if(category.contains(_name))
				return category[_name];
		}

		return FLT_MAX;
	}

	bool Config::GetBool(const string& _category, const string& _name)
	{
		if(m_bools.contains(_category))
		{
			auto category = m_bools[_category];
			if(category.contains(_name))
				return category[_name];
		}

		return false;
	}

	string Config::GetString(const string& _category, const string& _name)
	{
		if(m_strings.contains(_category))
		{
			auto category = m_strings[_category];
			if(category.contains(_name))
				return category[_name];
		}

		return "";
	}

	vec2 Config::GetVector(const string& _category, const string& _name)
	{
		if(m_vectors.contains(_category))
		{
			auto category = m_vectors[_category];
			if(category.contains(_name))
				return category[_name];
		}

		return {};
	}

	Color Config::GetColor(const string& _category, const string& _name)
	{
		if(m_colors.contains(_category))
		{
			auto category = m_colors[_category];
			if(category.contains(_name))
				return category[_name];
		}

		return {};
	}

	const char* Config::FilePath()
	{
		stringstream stream;

		stream << "./assets/";
		stream << m_fileName;
		stream << ".xml";

		m_fileName = new char[stream.str().size()];
		strcpy_s(m_fileName, stream.str().size() + 1, stream.str().c_str());

		return m_fileName;
	}

	bool Config::Initialise()
	{
		const xml_parse_result result = m_doc->load_file(FilePath());

		return result;
	}

	void Config::HandleVector(const string& _category, const string& _name, const string& _value)
	{
		auto converter = [](const string& _val) -> float
		{
			return std::stof(_val);
		};

		const vector<float> values = StringUtilities::Split<float>(_value, ',', converter);

		m_vectors[_category][_name] = vec2
		{
			values[0],
			values[1]
		};
	}

	void Config::HandleColor(const string& _category, const string& _name, const string& _value)
	{
		auto converter = [](const string& _val) -> byte
		{
			return static_cast<byte>(std::stoi(_val));
		};

		const vector<byte> values = StringUtilities::Split<byte>(_value, ',', converter);

		m_colors[_category][_name] = Color
		{
			values[0],
			values[1],
			values[2],
			values[3]
		};
	}

	EValType Config::StringToType(const string& _type)
	{
		if(_type == "int")
			return EValType::Int;

		if(_type == "float")
			return EValType::Float;

		if(_type == "bool")
			return EValType::Bool;

		if(_type == "string")
			return EValType::String;

		if(_type == "vector")
			return EValType::Vector;

		if(_type == "color")
			return EValType::Color;

		return EValType::Invalid;
	}
}
