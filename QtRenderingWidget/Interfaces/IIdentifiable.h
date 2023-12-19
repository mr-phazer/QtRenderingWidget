#pragma once
#include <string>

namespace Rldx {

	using IntId = uint32_t;
	static const IntId INVALID_ID = ~0;

	class IIdentifiable
	{
	public:
		IIdentifiable(const std::string& name) : m_id(GetNextId()), m_name(name) {}
		IIdentifiable() : m_id(GetNextId()) {}
		IntId GetId() const { return m_id; }
		std::string GetName() const { return m_name; }
		virtual std::string GetTypeString() = 0;

	private:
		static IntId GetNextId() { return sm_nextId++; }

	private:
		IntId m_id = INVALID_ID;
		static IntId sm_nextId;

	protected:
		std::string m_name = "Unnamed_Object";
	};

};