#pragma once
#include <string>
#include "..\Logging\Logging.h"

namespace Rldx {

	using IntId = uint32_t;
	static const IntId INVALID_ID = ~0;

	template  <typename ENUM_TYPE>
	class TIdentifiable
	{
	public:
		TIdentifiable(const std::string& name) : m_id(GetNextId()), m_name(name) {}
		TIdentifiable();
		virtual ~TIdentifiable();

		IntId GetId() const { return m_id; }

		std::string GetName() const { return m_name; }

		void SetName(const std::string& name)
		{
			m_name = /*GetTypeString() +*/ "__" + name + "_" + std::to_string(GetId());
		}

		// to force-add a descript, like "MeshNode"
		//virtual std::string GetTypeString() const = 0;

		virtual ENUM_TYPE GetType() const = 0;

	private: // in C++ is convention so seperate method from variable with and access modifier
		static IntId GetNextId() { return sm_nextId++; }

	private:
		IntId m_id = INVALID_ID;
		static IntId sm_nextId;

	protected:
		std::string m_name = "Unnamed_Object";
	};

	template<typename ENUM_TYPE>
	inline TIdentifiable<ENUM_TYPE>::TIdentifiable()
		: m_id(GetNextId())
	{
		// TODO: remove after debugging
#if _DEBUG
		logging::LogAction(/*GetTypeString()*/ + "# " + std::to_string(GetId()) + ": created.");
#endif
	}

	template<typename ENUM_TYPE>
	inline TIdentifiable<ENUM_TYPE>::~TIdentifiable()
	{
#if _DEBUG
		logging::LogAction(/*GetTypeString()*/ + "# " + std::to_string(GetId()) + ": deallocated.");
#endif
	};

	template<typename T>
	uint32_t TIdentifiable<T>::sm_nextId = 1;
};