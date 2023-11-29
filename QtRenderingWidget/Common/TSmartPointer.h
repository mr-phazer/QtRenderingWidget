#pragma
#include <memory>

namespace Rldx {
	template <typename CLASSNAME>
	struct TSmartPointerAll
	{
	public:
		using Unique = std::unique_ptr<CLASSNAME>;
		using Shared = std::shared_ptr<CLASSNAME>;
		using Weak = std::weak_ptr<CLASSNAME>;
	};

	template <typename CLASSNAME>
	struct TUniquePtr
	{	
		using UniquePtr = std::unique_ptr<CLASSNAME>;
		
		static UniquePtr MakeUnique()
		{
			return std::make_unique<CLASSNAME>();		
		}
	};


	template <typename CLASSNAME>
	struct TSmartPointerShared
	{
	public:
		using Shared = std::shared_ptr<CLASSNAME>;
	};


	template <typename CLASSNAME>
	struct TSmartPointerWeak
	{
	public:
		using Weak = std::weak_ptr<CLASSNAME>;
	};
}