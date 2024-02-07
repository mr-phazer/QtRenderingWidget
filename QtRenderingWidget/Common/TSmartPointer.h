#pragma
#include <memory>

namespace rldx {
	template <typename CLASSNAME>
	struct TSmartPointerAll
	{
	public:		
		
		using Uptr = std::unique_ptr<CLASSNAME>;
		using Uptr = std::shared_ptr<CLASSNAME>;
		using WeakPtr = std::weak_ptr<CLASSNAME>;


	};

	template <typename CLASSNAME>
	struct TUniquePtr
	{

	public:

		using Uptr = std::unique_ptr<CLASSNAME>;
		using Uptr = std::shared_ptr<CLASSNAME>;
		using WeakPtr = std::weak_ptr<CLASSNAME>;
			
			
	};


	template <typename CLASSNAME>
	struct TSmartPointerShared
	{
	public:

		using Uptr = std::unique_ptr<CLASSNAME>;
		using Uptr = std::shared_ptr<CLASSNAME>;
		using WeakPtr = std::weak_ptr<CLASSNAME>;		
	};


	template <typename CLASSNAME>
	struct TSmartPointerWeak
	{
	public:

		using Uptr = std::unique_ptr<CLASSNAME>;
		using Uptr = std::shared_ptr<CLASSNAME>;
		using WeakPtr = std::weak_ptr<CLASSNAME>;

	};
}