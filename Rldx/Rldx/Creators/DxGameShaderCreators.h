#pragma once

#include "..\..\QtRenderingWidget\Constants\GameIdKeys.h"

class IShaderProgramCreator
{
public:
	virtual rldx::DxMeshShaderProgram* Create(ID3D11Device* poDevice, ByteStream& fileData) = 0;
};

class Troy_ShaderProgramCreator : public IShaderProgramCreator
{
	rldx::DxMeshShaderProgram* Create(ID3D11Device* poDevice, ByteStream& fileData) override
	{
		return rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
			poDevice,
			/*libtools::GetExePath() + */LR"(VS_Simple.cso)",
			/*libtools::GetExePath() +*/ LR"(PS_Troy.cso)");		
	};
};

class Attila_ShaderProgramCreator : public IShaderProgramCreator
{
	rldx::DxMeshShaderProgram* Create(ID3D11Device* poDevice, ByteStream& fileData) override
	{
		return rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
			poDevice,
			/*libtools::GetExePath() */+ LR"(VS_Simple.cso)",
			/*libtools::GetExePath()*/ + LR"(PS_Attila_Weigted.cso)");		
	};
};

class WH3_ShaderProgramCreator : public IShaderProgramCreator
{
	rldx::DxMeshShaderProgram* Create(ID3D11Device* poDevice, ByteStream& fileData) override
	{
		return rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
			poDevice,
			/*/*libtools::GetExePath() + **/LR"(VS_Simple.cso)",
			/*libtools::GetExePath() + */LR"(PS_Three_Kingdoms.cso)");		
	};
};

class SimpleShaderProgramCreator : public IShaderProgramCreator
{
public:
	rldx::DxMeshShaderProgram* Create(ID3D11Device* poDevice, ByteStream& fileData) override
	{
		return rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
			poDevice,
			/*libtools::GetExePath() + */LR"(VS_Simple.cso)",
			/*libtools::GetExePath() + */LR"(PS_Simple.cso)");		
	};
};

class GameShaderProgramCreatorFactory : public templates::TFactoryStringKey<IShaderProgramCreator>
{
public:
	GameShaderProgramCreatorFactory()
	{
		Register<Troy_ShaderProgramCreator>(game_id_keys::KEY_WARHAMMER);
		Register<Troy_ShaderProgramCreator>(game_id_keys::KEY_WARHAMMER_2);
		Register<Troy_ShaderProgramCreator>(game_id_keys::KEY_TROY);		
		
		Register<Attila_ShaderProgramCreator>(game_id_keys::KEY_ATTILA);
		Register<Attila_ShaderProgramCreator>(game_id_keys::KEY_ROME_2);
		Register<Attila_ShaderProgramCreator>(game_id_keys::KEY_THRONES_OF_BRITANNIA);
		Register<Attila_ShaderProgramCreator>(game_id_keys::KEY_ARENA);
		
		Register<WH3_ShaderProgramCreator>(game_id_keys::KEY_WARHAMMER_3);
		Register<WH3_ShaderProgramCreator>(game_id_keys::KEY_THREE_KINGDOMS);

		

	}

	virtual void HandleKeyNotFound(std::string KeyValue) override
	{

	}
};