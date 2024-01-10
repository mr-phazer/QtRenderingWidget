#pragma once

#include <map>
#include <exception>
#include <memory>


namespace helpers
{

	/// <summary>
	/// Dictionary class ala C# Dictionary
	/// VALUE are stored in a unique_ptr map
	/// </summary>
	/// <typeparam name="KEY">Use an enum (non-class is propably easier for binary input) or integer</typeparam>
	/// <typeparam name="VALUE">use class</typeparam>
	template <typename KEY, typename VALUE>
	class Dictionary
	{
		std::map<KEY, std::unique_ptr<VALUE>> m_map;
		VALUE& GetValue(const KEY& key)
		{
			auto it = m_map.find(key);

			if (it != m_map.end())
			{
				return it->first;
			}
			else
			{
				throw std::exception("Key not found");
			}
		}

	public:
		Dictionary() = default;
		Dictionary(const std::map<KEY, VALUE*>& initMap)
			: m_map(initMap)
		{
		}

		const VALUE* Get(KEY value) const
		{
			const auto Key = GetKey(value);
			return Key;
		}

		VALUE* Get(KEY value)
		{
			auto Key = GetKey(value);
			return Key;
		}

		const VALUE* operator[](KEY key) const
		{
			const auto Key = GetKey(value);
			return Key;
		}

		VALUE* operator[](KEY key)
		{
			const auto Key = GetKey(value);
			return Key;
		}
	};

};