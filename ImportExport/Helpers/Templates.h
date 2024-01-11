#pragma once

#include <map>

//////////////////////////////////////////////////////////////////////////
namespace helpers
{

	template <typename T>
	class TByteCreator
	{
		virtual T Create(ByteStream& stream) = 0;
	};

	//////////////////////////////////////////////////////////////////////////

	template <typename BASE, typename ENUM>
	class TFactory
	{
		std::map<ENUM, std::unique_ptr<BASE>> m_map;

	public:
		template<typename DERIVED>
		void Register(ENUM Type, DERIVED* poRawPtr)
		{
			m_map[Type] = std::unique_ptr<DERIVED>(poRawPtr);
		}

		BASE* Get(ENUM Type)
		{
			auto it = m_map.find(Type);
			if (it == m_map.end())
				throw std::exception("TFactory::Create: Type not found");

			return it->second.get();
		}
	};
} // namespace helpers

//////////////////////////////////////////////////////////////////////////