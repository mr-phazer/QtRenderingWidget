#pragma once

#include <d3d11.h>

#include "..\DataTypes\DxMeshData.h"
#include "..\Interfaces\IBindable.h"
#include "..\Interfaces\IDrawable.h"
#include "..\Managers\ResourceManager\IDxResource.h"

namespace rldx
{
	/// <summary>
	/// Contains the mesh data. Can bind and draw it
	/// </summary>
	class DxMesh : public IDxResource, public IDrawable, public IBindable
	{
	public:
		DxMesh(const std::wstring& meshName = L"UnnamesMesh")
		{
			SetName(meshName);
			SetType(ResourceTypeEnum::Mesh);
			SetTypeString(L"Resource:Mesh");
		}

		void Draw(ID3D11DeviceContext* poDC) override;
		void BindToDC(ID3D11DeviceContext* poDC) override;

		void SetMeshData(const DxCommonMeshData& meshData);

		const DirectX::XMFLOAT3& GetMin() const;
		const DirectX::XMFLOAT3& GetMax() const;

	private:
		DxCommonMeshData m_poMeshBuffers;
	};

}; // namespace rldx
