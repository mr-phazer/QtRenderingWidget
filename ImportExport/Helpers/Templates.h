#pragma once

#include <map>
#include <memory>
#include "..\..\ImportExport\Helpers\ByteStream.h"
#include "..\..\Rldx\Rldx\Helpers\StringKeyMap.h"

//////////////////////////////////////////////////////////////////////////
namespace templates
{
	template <typename DATA_TYPE>
	class TCreator
	{
		virtual DATA_TYPE Create(ByteStream& stream) = 0;
	};

	//////////////////////////////////////////////////////////////////////////



	// TODO: make b template + template a create class,  so one doesn't have to implement creators manually
	template <typename BASE_TYPE, typename KEY_TYPE>
	class TAbstractFactory
	{
		std::map<KEY_TYPE, std::shared_ptr<BASE_TYPE>> m_map;

	public:

		bool ContainsKey(KEY_TYPE KeyValue)
		{
			return (m_map.find(KeyValue) != m_map.end);
		}

		template<KEY_TYPE KeyValue, typename DERIVED_TYPE>
		void Register()
		{
			m_map[KeyValue] = std::make_shared<DERIVED_TYPE>();
		}

		std::shared_ptr<BASE_TYPE> Get(KEY_TYPE KeyValue)
		{
			auto it = m_map.find(KeyValue);

			if (it == m_map.end()) {
				HandleKeyNotFound(KeyValue);
				return nullptr;
			}

			return it->second;
		}

		/// <summary>
		/// Client forced tp supply own handling
		/// </summary>		
		virtual void HandleKeyNotFound(KEY_TYPE keyValue) = 0;
	};

	template <typename BASE_TYPE>
	class TFactoryStringKey
	{
		StringkeyMap<std::shared_ptr<BASE_TYPE>> m_map;

	public:
		// TODO: remove is othre method is better
		//template<KeyValuename DERIVED_TYPE>
		//void Register(KEY_TYPE KeyValue, DERIVED_TYPE* poRawPtr)
		//{
		//	m_map[KeyValue] = std::shared_ptr<DERIVED_TYPE>(poRawPtr);
		//}

		bool ContainsKey(std::string KeyValue)
		{
			return (m_map.find(KeyValue) != m_map.end);
		}

		template<typename DERIVED_TYPE>
		void Register(const std::string& KeyValue)
		{
			m_map[KeyValue] = std::make_shared<DERIVED_TYPE>();
		}

		std::shared_ptr<BASE_TYPE> Get(const std::string& KeyValue)
		{
			auto it = m_map.find(KeyValue);

			if (it == m_map.end()) {
				HandleKeyNotFound(KeyValue);
				return nullptr;
			}

			return it->second;
		}

		virtual void HandleKeyNotFound(std::string KeyValue) = 0;


	};

	template <typename BASE_TYPE>
	class TFactoryWStringKey
	{
		WStringkeyMap<std::shared_ptr<BASE_TYPE>> m_map;

	public:
		// TODO: remove is othre method is better
		//template<KeyValuename DERIVED_TYPE>
		//void Register(KEY_TYPE KeyValue, DERIVED_TYPE* poRawPtr)
		//{
		//	m_map[KeyValue] = std::shared_ptr<DERIVED_TYPE>(poRawPtr);
		//}

		bool ContainsKey(std::wstring KeyValue)
		{
			return (m_map.find(KeyValue) != m_map.end);
		}

		template<typename DERIVED_TYPE>
		void Register(const std::wstring& KeyValue)
		{
			m_map[KeyValue] = std::make_shared<DERIVED_TYPE>();
		}

		std::shared_ptr<BASE_TYPE> Get(const std::wstring& KeyValue)
		{
			auto it = m_map.find(KeyValue);

			if (it == m_map.end()) {
				HandleKeyNotFound(KeyValue);
				return nullptr;
			}

			return it->second;
		}

		virtual void HandleKeyNotFound(std::wstring KeyValue) = 0;

	};


} // namespace templates

//////////////////////////////////////////////////////////////////////////