#pragma once

#include <map>
#include "..\..\Rldx\Rldx\Helpers\StringKeyMap.h"

//////////////////////////////////////////////////////////////////////////
namespace helpers
{

	template <typename CONST_BUF_DATA_KeyValue>
	class TByteCreator
	{
		virtual CONST_BUF_DATA_KeyValue Create(ByteStream& stream) = 0;
	};

	//////////////////////////////////////////////////////////////////////////

	template <typename BASE_TYPE, typename KEY_TYPE>
	class TFactory
	{
		std::map<KEY_TYPE, std::shared_ptr<BASE_TYPE>> m_map;

	public:
		// TODO: remove is othre method is better
		//template<KeyValuename DERIVED_TYPE>
		//void Register(KEY_TYPE KeyValue, DERIVED_TYPE* poRawPtr)
		//{
		//	m_map[KeyValue] = std::unique_ptr<DERIVED_TYPE>(poRawPtr);
		//}

		bool IsKeyKnown(KEY_TYPE KeyValue)
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

		virtual void HandleKeyNotFound(KEY_TYPE KeyValue)
		{			
		}

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
		//	m_map[KeyValue] = std::unique_ptr<DERIVED_TYPE>(poRawPtr);
		//}

		bool IsKeyKnown(std::string KeyValue)
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

		virtual void HandleKeyNotFound(std::string KeyValue)
		{			
		}

	};

} // namespace helpers

//////////////////////////////////////////////////////////////////////////