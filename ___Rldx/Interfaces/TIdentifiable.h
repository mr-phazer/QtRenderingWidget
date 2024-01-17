#pragma once
#include <string>
#include "..\..\Logging\Logging.h"

namespace rldx {

	using IntId = uint32_t;
	static const IntId INVALID_ID = ~0;

	class IdentifiableBase
	{
	public:
		IdentifiableBase() : m_id(GetNextId()) {}
		virtual ~IdentifiableBase() {};

		IntId GetId() const { return m_id; }		

	private: // in C++ is convention so seperate method from variable with and access modifier
		static IntId GetNextId() { return sm_nextId++; }

	private:
		IntId m_id = INVALID_ID;
		static IntId sm_nextId;
	};

	template  <typename ENUM_TYPE>
	class TIdentifiable : public IdentifiableBase

	{
	public:
		TIdentifiable(const std::string& name) : IdentifiableBase(), m_name(name) {}
		TIdentifiable();
		virtual ~TIdentifiable();

		virtual std::string GetTypeString() const = 0;
		virtual ENUM_TYPE GetType() const = 0;

	protected:
		std::string m_name = "Unnamed_Object";
	};

	template<typename ENUM_TYPE>
	inline TIdentifiable<ENUM_TYPE>::TIdentifiable()
		: IdentifiableBase()
	{
		// TODO: remove after debugging
#if _DEBUG
		logging::LogAction(/*this->GetTypeString() + */"# " + std::to_string(GetId()) + ": created.");
#endif
	}

	template<typename ENUM_TYPE>
	inline TIdentifiable<ENUM_TYPE>::~TIdentifiable()
	{
#if _DEBUG
		logging::LogAction(/*GetTypeString()*/ +"# " + std::to_string(GetId()) + ": deallocated.");
#endif
	};	
};