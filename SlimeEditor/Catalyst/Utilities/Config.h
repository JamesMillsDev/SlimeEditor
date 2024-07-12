#pragma once

#include <string>
#include <istream>
#include <map>

#include <glm/vec2.hpp>

#include "Color.h"

using std::string;
using std::map;

namespace pugi
{
	class xml_document;
}

using pugi::xml_document;

using glm::vec2;

namespace Catalyst::Utilities
{
	enum class EValType : int8_t
	{
		Invalid = -1,
		Int,
		Float,
		Bool,
		String,
		Vector,
		Color
	};

	class Config
	{
	public:
		Config(char* _name);
		~Config();

	public:
		void Load();

		int GetInt(const string& _category, const string& _name);
		float GetFloat(const string& _category, const string& _name);
		bool GetBool(const string& _category, const string& _name);
		string GetString(const string& _category, const string& _name);
		vec2 GetVector(const string& _category, const string& _name);
		Color GetColor(const string& _category, const string& _name);

	private:
		xml_document* m_doc;
		char* m_fileName;

		map<string, map<string, int>> m_ints;
		map<string, map<string, float>> m_floats;
		map<string, map<string, bool>> m_bools;
		map<string, map<string, string>> m_strings;
		map<string, map<string, vec2>> m_vectors;
		map<string, map<string, Color>> m_colors;

	private:
		const char* FilePath();

		bool Initialise();

		void HandleVector(const string& _category, const string& _name, const string& _value);
		void HandleColor(const string& _category, const string& _name, const string& _value);

	private:
		static EValType StringToType(const string& _type);

	};
}
