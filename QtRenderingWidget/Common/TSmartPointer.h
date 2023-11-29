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
	struct TSmartPointerUnique
	{
	public:
		using Unique = std::unique_ptr<CLASSNAME>;
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