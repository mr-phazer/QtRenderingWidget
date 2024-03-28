#pragma once

#include "..\..\ImportExport\Helpers\Templates.h"
#include "..\..\QtRenderingWidget\Constants\GameIdKeys.h"
#include "..\Rendering\DxShaderProgram.h"

namespace rldx
{
	const std::wstring VertexShader_Weighted4_Path = LR"(VS_Weighted4.cso)";
	const std::wstring VertexShader_Default_Path = LR"(VS_Weighted4.cso)";

	class IShaderProgramCreator
	{
	public:
		virtual DxMeshShaderProgram* Create(ID3D11Device* poDevice) = 0;
	};

	class Troy_ShaderProgramCreator : public IShaderProgramCreator
	{
		DxMeshShaderProgram* Create(ID3D11Device* poDevice) override
		{
			return DxMeshShaderProgram::Create<DxMeshShaderProgram>(
				poDevice,
				LR"(VS_Weighted4.cso)",
				LR"(PS_Troy.cso)");
		};
	};

	class Attila_ShaderProgramCreator : public IShaderProgramCreator
	{
		DxMeshShaderProgram* Create(ID3D11Device* poDevice) override
		{
			return DxMeshShaderProgram::Create<DxMeshShaderProgram>(
				poDevice,
				LR"(VS_Weighted4.cso)",
				LR"(PS_Attila_Weigted.cso)");
		};
	};

	class WH3_ShaderProgramCreator : public IShaderProgramCreator
	{
		DxMeshShaderProgram* Create(ID3D11Device* poDevice) override
		{
			return DxMeshShaderProgram::Create<DxMeshShaderProgram>
				(
					poDevice,
					LR"(VS_Weighted4.cso)",
					LR"(PS_Three_Kingdoms.cso)"
				);
		};
	};

	class SimpleShaderProgramCreator : public IShaderProgramCreator
	{
	public:
		DxMeshShaderProgram* Create(ID3D11Device* poDevice) override
		{
			return DxMeshShaderProgram::Create<DxMeshShaderProgram>
				(
					poDevice,
					LR"(VS_Weighted4.cso)",
					LR"(PS_Simple.cso)"
				);
		};
	};



	enum class ShaderTypeEnum
	{
		TroyWeighted, TroyDefault,
		Wh3Weighted, Wh3Default,
		AttilaWeighted, AtillaDefault,
	};

	enum class VertexShaderType
	{
		Weighted, NonWeighted
	};


	// TODO: change this to use the "ShaderTypeEnum" ENUMs instead
	class GameShaderProgramCreatorFactory : public templates::TFactoryWStringKey<IShaderProgramCreator>
	{
	public:
		// TODO: use game_id_string and weighted true/false, to return the correct enum
		ShaderTypeEnum GetTypeId(std::wstring gameIdString, bool weightedVs);

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
		};

		virtual void HandleKeyNotFound(std::wstring KeyValue) override
		{
			throw std::exception(FULL_FUNC_INFO("Error Creating Shader: Incorrect GameIdString").c_str());
		}
	};
}