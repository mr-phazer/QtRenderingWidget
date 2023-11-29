#include <iostream>


namespace Rldx
{
	template <typename DATA_TYPE>
	class DxMaterialManager
	{
		DATA_TYPE* GetResouce(uint32_t index);
		uint32_t SetMaterial(DxMaterial* material)
		{

		}

	private:
		std::unordered_map<uint32_t, DATA_TYPE*> m_materials;
	}; 

	template <typename DATA_TYPE>
	class TDxResourceManagerTemplate {
	private:
		DATA_TYPE Data;

		static TDxResourceManagerTemplate* instance;

		TDxResourceManagerTemplate() {
			// private constructor to prevent instantiation
		}

	public:
		static TDxResourceManagerTemplate* GetInstance() {
			if (instance == nullptr) {
				instance = new TDxResourceManagerTemplate();
			}
			return instance;
		}

		
	};

	TDxResourceManagerTemplate* TDxResourceManagerTemplate::instance = nullptr;


}
