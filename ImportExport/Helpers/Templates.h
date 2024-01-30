#pragma once

#include <map>

//////////////////////////////////////////////////////////////////////////
namespace helpers
{

	template <typename CONST_BUF_DATA_TYPE>
	class TByteCreator
	{
		virtual CONST_BUF_DATA_TYPE Create(ByteStream& stream) = 0;
	};

	//////////////////////////////////////////////////////////////////////////

	template <typename BASE, typename ENUM>
	class TFactory
	{
		std::map<ENUM, std::shared_ptr<BASE>> m_map;

	public:
		template<typename DERIVED>
		void Register(ENUM Type, DERIVED* poRawPtr)
		{
			m_map[Type] = std::unique_ptr<DERIVED>(poRawPtr);
		}

		std::shared_ptr<BASE> Get(ENUM Type)
		{
			auto it = m_map.find(Type);
			if (it == m_map.end())
				throw std::exception("TFactory::Create: Type not found");

			return it->second;
		}
	};
	
} // namespace helpers

//////////////////////////////////////////////////////////////////////////