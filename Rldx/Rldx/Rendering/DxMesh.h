#pragma once

#include <d3d11.h>

#include "..\DataTypes\DxMeshData.h"
#include "..\Interfaces\IBindable.h"
#include "..\Interfaces\IDrawable.h"
#include "..\Managers\DxTypes.h"
#include "..\Managers\ResourceManager\IDxResource.h"


namespace rldx
{
	/// <summary>
	/// Contains the mesh data. Can bind and draw it
	/// </summary>
	class DxMesh : public IDxResource, public IDrawable, public IBindable
	{
	public:
		void Draw(ID3D11DeviceContext* poDC) override;;
		void SetMeshData(const DxCommonMeshData& meshData);
		void BindToDC(ID3D11DeviceContext* poDC);;

		ResourceTypeEnum GetType() const override;
		std::wstring GetTypeString() const override;

		DirectX::XMFLOAT3 GetMin();
		DirectX::XMFLOAT3 GetMax();

	private:
		DxCommonMeshData m_poMeshBuffers;
	};

}; // namespace rldx
