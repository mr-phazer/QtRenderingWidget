#pragma
#include <memory>

namespace Rldx {
	template <typename CLASSNAME>
	struct TSmartPointerAll
	{
	public:		
		
		using UniquePtr = std::unique_ptr<CLASSNAME>;
		using SharedPtr = std::shared_ptr<CLASSNAME>;
		using WeakPtr = std::weak_ptr<CLASSNAME>;


	};

	template <typename CLASSNAME>
	struct TUniquePtr
	{

	public:

		using UniquePtr = std::unique_ptr<CLASSNAME>;
		using SharedPtr = std::shared_ptr<CLASSNAME>;
		using WeakPtr = std::weak_ptr<CLASSNAME>;
			
			
	};


	template <typename CLASSNAME>
	struct TSmartPointerShared
	{
	public:

		using UniquePtr = std::unique_ptr<CLASSNAME>;
		using SharedPtr = std::shared_ptr<CLASSNAME>;
		using WeakPtr = std::weak_ptr<CLASSNAME>;


		static auto MakeShared = std::make_shared<CLASSNAME>;
	};


	template <typename CLASSNAME>
	struct TSmartPointerWeak
	{
	public:

		using UniquePtr = std::unique_ptr<CLASSNAME>;
		using SharedPtr = std::shared_ptr<CLASSNAME>;
		using WeakPtr = std::weak_ptr<CLASSNAME>;

	};
}