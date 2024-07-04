#pragma once
#include <string>
#include "Logger\Logger.h"

namespace rldx {

	using namespace utils;

	using IntId = uint32_t;
	static const IntId INVALID_ID = ~static_cast<IntId>(0);

	class IdentifiableBase
	{
	public:
		IdentifiableBase() : m_id(GetNextId()) {}
		virtual ~IdentifiableBase() {};

		IntId GetId() const { return m_id; }

		/// <summary>
		/// Manually set id. Only use if you know what you are doing.
		/// </summary>
		void SetId(IntId id) { m_id = id; };

	private: // in C++ is convention so seperate method from variable with and access modifier
		static IntId GetNextId() { return sm_nextId++; }


	private:
		IntId m_id = INVALID_ID;
		static IntId sm_nextId;
	};

	template  <typename KEY_TYPE>
	class TIdentifiable : public IdentifiableBase
	{
	public:
		TIdentifiable(const std::wstring& name) : IdentifiableBase(), name(name) {}

		//TODO: have private default constructor, to force derived classes to set Type Enum/Type description in the constructor
		TIdentifiable();
		virtual ~TIdentifiable();

		// TODO: Scrap these, and make the derived classes set Type Enum/Type description in the constructor
		virtual std::wstring GetTypeString() const { return L"TIdentifiable<T>"; }
		virtual KEY_TYPE GetType() const = 0;
		virtual std::wstring GetIdString() const { return GetTypeString() + L"_" + std::to_wstring(GetId()); }

	protected:
		std::wstring name = L"Unnamed_Object";
	};

	template<typename KEY_TYPE>
	inline TIdentifiable<KEY_TYPE>::TIdentifiable()
		: IdentifiableBase()
	{
		// TODO: remove after debugging
#if _DEBUG
		logging::LogAction(GetTypeString() + L"# " + std::to_wstring(GetId()) + L": created.");
#endif
	}

	template<typename KEY_TYPE>
	inline TIdentifiable<KEY_TYPE>::~TIdentifiable()
	{
#if _DEBUG
		logging::LogAction(GetTypeString() + L"# " + std::to_wstring(GetId()) + L": deallocated.");
#endif
	};
};