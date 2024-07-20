#pragma once
#include <string>
#include <CommonLibs/Logger/Logger.h>

namespace rldx {

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
		TIdentifiable();
		virtual ~TIdentifiable();

		// TODO: Scrap these, and make the derived classes set Type Enum/Type description in the constructor
		std::wstring GetName() const { return m_name; }
		std::wstring GetTypeString() const { return m_typeString; }
		std::wstring GetIdString() const { return GetTypeString() + L"_" + std::to_wstring(GetId()); }
		KEY_TYPE GetType() const { return m_nodeType; }

	protected:
		void SetType(KEY_TYPE type) { m_nodeType = type; }
		void SetTypeString(const std::wstring& typeString) { m_typeString = typeString; }
		void SetName(const std::wstring& name) { m_name = name; }

	private:
		std::wstring m_name = L"Unnamed_Object";
		std::wstring m_typeString = L"TIdentifiable<T>";
		KEY_TYPE m_nodeType;
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