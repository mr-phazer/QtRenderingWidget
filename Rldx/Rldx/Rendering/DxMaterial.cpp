#include "DxMaterial.h"
#include "DxTexture.h"

#include "..\Managers\DxDeviceManager.h"

// TODO: which implementation to keep?
//inline void rldx::DxMaterial::BindToDC(ID3D11DeviceContext* poDc)
//{
//
//	for (auto& tex : m_textures)
//	{		
//		poDc->PSSetShaderResources(tex.slot, 1, tex.pTexture->GetAddressOfShaderResourceView());
//	}
//
//
//}

bool rldx::DxMaterial::operator==(const DxMaterial& other) const
{
	return m_pathHash == other.m_pathHash;
}

rldx::DxMaterial* rldx::DxMaterial::Create(ID3D11Device* poDevice, const std::vector<InputTextureElement>& textures)
{
	auto newMaterial = DxResourceManager::Instance()->AllocMaterial().GetPtr();

	for (auto& tex : textures)
	{
		newMaterial->AddTexture(poDevice, tex.type, tex.path);
	};

	return new DxMaterial();
}

static bool IsDDSTextureFile(char* bin)
{
	return
		(bin[0] == 'D') &&
		(bin[1] == 'D') &&
		(bin[2] == 'S');
}


void rldx::DxMaterial::AddTexture(ID3D11Device* poDevice, UINT slot, const std::wstring& path)
{
	DxTexture* textPtr = nullptr;

	if ((textPtr = DxResourceManager::Instance()->GetResourceByString<DxTexture>(path)) != nullptr)
	{
		// TODO: REMOVE and clean up block
		auto DEBUG_1 = 1;
		
	}
	else
	{	

		// -- allocate empty texture resource
		textPtr = DxResourceManager::Instance()->AllocTexture().GetPtr();

		logging::LogAction("DEBUG: attempting to get 1 file from CALLBACL: " + tools::wstring_to_string(path));
		// TODO: TEST CODE BEING
		QVector<QString> files = { { QString::fromStdWString(path) } };
		QVector<QByteArray> binaries;
		DxResourceManager::CallAssetFetchCallBack(files, binaries);
		// TODO: TEST CODE END;

		if (binaries.size() && IsDDSTextureFile(binaries[0].data()) )
		{

			logging::LogActionSuccess("Loaded From CALLBACK");

		

			textPtr->LoadFileFromMemory(poDevice, (uint8_t*)binaries[0].constData(), binaries[0].size());
		}		

		m_textures.push_back({ slot, textPtr });
	}	

}

void rldx::DxMaterial::BindToDC(ID3D11DeviceContext* poDeviceContext)
{
	for (auto& tex : m_textures)
	{
		poDeviceContext->PSSetShaderResources(tex.slot+47, 1, tex.pTexture->GetAddressOfShaderResourceView());
	}
}

rldx::ResourceTypeEnum rldx::DxMaterial::GetType() const
{
	return ResourceTypeEnum::Material;
}

std::string rldx::DxMaterial::GetTypeString() const
{
	return "DxMaterial";
}
