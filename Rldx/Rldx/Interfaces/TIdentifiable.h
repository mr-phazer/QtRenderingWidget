#pragma once
#include <string>
#include <CommonLibs/Logger/Logger.h>

namespace rldx {

	using IntId = uint32_t;
	static const IntId INVALID_ID = ~static_cast<IntId>(0);



	class UniqueIdGenerator
	{
		static IntId sm_nextId;
	public:
		static IntId GetNextId() { return sm_nextId++; }
	};

	class IdentifiableBase
	{
	public:
		IdentifiableBase() : m_id(UniqueIdGenerator::GetNextId()) {}
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

		void WriteDebugDestructMsg();
		void WriteDebugConstructMsg();

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
		std::wstring m_name = L"no_name";
		std::wstring m_typeString = L"Tntifiable<T>";
		KEY_TYPE m_nodeType;
	};

	template<typename KEY_TYPE>
	inline TIdentifiable<KEY_TYPE>::TIdentifiable()
		: IdentifiableBase()
	{
		WriteDebugConstructMsg();
	}

	template<typename KEY_TYPE>
	inline TIdentifiable<KEY_TYPE>::~TIdentifiable()
	{
		WriteDebugDestructMsg();
	}

	/// <summary>
	/// Writes construction message wiht id, id string, name, for help with debuggin potential memory leaks
	/// </summary>	
	template<typename KEY_TYPE>
	inline void TIdentifiable<KEY_TYPE>::WriteDebugDestructMsg()
	{
#ifdef _DEBUG
		//logging::Logger::LogSimpleWithColor(L"Object Id: " + std::to_wstring(GetId()) + L", Type: " + GetTypeString() + L", Name : " + GetName() + L", : Destroyed.", logging::BG_BLACK | logging::FG_GRAY);
#endif
	}

	/// <summary>
	/// Writes destruction message wiht id, id string, name, for help with debuggin potential memory leaks
	/// </summary>	
	template<typename KEY_TYPE>
	inline void TIdentifiable<KEY_TYPE>::WriteDebugConstructMsg()
	{
#ifdef _DEBUG
		//logging::Logger::LogSimpleWithColor(L"Object Id: " + std::to_wstring(GetId()) + L", Type: " + GetTypeString() + L", Name : " + GetName() + L", : Constructed.", logging::BG_BLACK | logging::FG_DARKGRAY);
#endif
	};
}