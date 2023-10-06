#include "GraphicsPipelineManager.h"
#include "SafeDelete.h"
#include "Helpar.h"
#include <string>

using namespace Microsoft::WRL;

ID3D12Device* GraphicsPipelineManager::device = nullptr;
std::unordered_map<std::string, GraphicsPipelineManager::GRAPHICS_PIPELINE> GraphicsPipelineManager::graphicsPipeline;
GraphicsPipelineManager::DrawSet GraphicsPipelineManager::oldDraw;

void SetLayout(D3D12_INPUT_ELEMENT_DESC* inputLayout,const std::vector<std::string>& _inputLayoutType)
{
	const D3D12_INPUT_CLASSIFICATION input = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

	//layout��
	const int size = int(_inputLayoutType.size());

	// ���_���C�A�E�g�̐ݒ�
	for (int i = 0; i < size; i++)
	{
		//���W
		if (_inputLayoutType[i] == "POSITION")
		{
			inputLayout[i] = {
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,input, 0 };
		}
		//�@��
		else if (_inputLayoutType[i] == "NORMAL")
		{
			inputLayout[i] = {
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,input, 0 };
		}
		//2Duv���W
		else if (_inputLayoutType[i] == "TEXCOORD_2D")
		{
			inputLayout[i] = {
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,input, 0 };
		}
		//�{�[���ԍ�
		else if (_inputLayoutType[i] == "BONEINDICES")
		{
			inputLayout[i] = {
				"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,input, 0 };
		}
		//�{�[���e���x
		else if (_inputLayoutType[i] == "BONEWEIGHTS")
		{
			inputLayout[i] = {
				"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,input, 0 };
		}
		//�X�P�[��
		else if (_inputLayoutType[i] == "SCALE")
		{
			inputLayout[i] = {
				"SCALE", 0, DXGI_FORMAT_R32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,input, 0 };
		}
		//�F
		else if (_inputLayoutType[i] == "COLOR")
		{
			inputLayout[i] = {
				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,input, 0 };
		}
	}
}

D3D12_RENDER_TARGET_BLEND_DESC GraphicsPipelineManager::CreateBlendDesc(const std::string& _mode)
{
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc;
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��
	if (_mode == "NOBLEND")
	{
		blenddesc.BlendEnable = false;
		return blenddesc;
	}

	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	if (_mode == "ALPHA")
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	} else if (_mode == "ADD")
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
	} else if (_mode == "SUB")
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
	} else if (_mode == "MULA")
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ZERO;
		blenddesc.DestBlend = D3D12_BLEND_INV_DEST_COLOR;
	} else if (_mode == "INVSRC")
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
	}
	blenddesc.LogicOpEnable = false;

	return blenddesc;
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC GraphicsPipelineManager::CreatepelineDesc(const PEPELINE& _pepeline)
{
	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//�V�F�[�_�[�̃Z�b�g
	for (auto& i : _pepeline.shaderType) {
		if (i == "v") {
			gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObject[_pepeline.name + "VS"].Get());
		} else if (i == "p") {
			gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObject[_pepeline.name + "PS"].Get());
		} else if (i == "g") {
			gpipeline.GS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObject[_pepeline.name + "GS"].Get());
		}
	}

	//�T���v���}�X�N�ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//�W���ݒ�
	//���X�^���C�U�X�e�[�g�ݒ�
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = CreateBlendDesc(_pepeline.blendMode);
	for (int i = 0; i < _pepeline.rtvNum; i++)
	{
		gpipeline.BlendState.RenderTarget[i] = blenddesc;
	}

	//�[�x�l�t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//���_���C�A�E�g�̐ݒ�
	D3D12_INPUT_ELEMENT_DESC* inputLayout = new D3D12_INPUT_ELEMENT_DESC[int(_pepeline.inputLayoutType.size())];
	SetLayout(inputLayout, _pepeline.inputLayoutType);
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements =UINT(_pepeline.inputLayoutType.size());

	//�}�`�̌`����O�p�`�ɐݒ�
	if (_pepeline.drawType == "POINT") {
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	}else if (_pepeline.drawType == "LINE_LIST"|| _pepeline.drawType == "LINE_STRIP") {
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	}else if (_pepeline.drawType == "TRIANGLE_LIST"|| _pepeline.drawType == "TRIANGLE_STRIP") {
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	}

	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = _pepeline.rtvNum;	//�`��Ώۂ�1��
	for (int i = 0; i < _pepeline.rtvNum; i++) {
		gpipeline.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;	//0�`255�w���RGBA
	}
	gpipeline.SampleDesc.Count = 1;	//1�s�N�Z���ɂ�1��T���v�����O

	//2D�`��Ȃ�㏑�����[�h�ɐݒ�
	if (_pepeline.drawMode == "2D")
	{
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		gpipeline.DepthStencilState.DepthEnable = false;
	} else if (_pepeline.drawMode == "PARTICLE")
	{
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}

	return gpipeline;
}

void GraphicsPipelineManager::CreateRootSignature(const std::string& _pepelineName, const PEPELINE& _pepeline)
{
	HRESULT result = S_FALSE;

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc;

	std::vector<CD3DX12_ROOT_PARAMETER> rootparams(_pepeline.rootparams + _pepeline.textureNum);
	for (int i = 0; i < _pepeline.rootparams; i++) {
		rootparams[i].InitAsConstantBufferView(i, 0, D3D12_SHADER_VISIBILITY_ALL);
	}

	// �f�X�N���v�^�����W
	const int tex_num = _pepeline.textureNum;
	CD3DX12_DESCRIPTOR_RANGE* descRangeSRV = new CD3DX12_DESCRIPTOR_RANGE[tex_num];
	for (int i = 0; i < tex_num; i++)
	{
		// �f�X�N���v�^�����W
		descRangeSRV[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i); //t�� ���W�X�^

		// SRV�i�e�N�X�`���j
		int paramNum = _pepeline.rootparams + i;
		rootparams[paramNum].InitAsDescriptorTable(1, &descRangeSRV[i], D3D12_SHADER_VISIBILITY_ALL);
	}

	//�e�N�X�`���T���v���[�̐ݒ�
	if (_pepeline.drawMode == "2D"){
		samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	} else {
		samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 1);
	}
	//���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0((UINT)rootparams.size(), rootparams.data(), 1,
		&samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//���[�g�V�O�l�`���̐���
	ComPtr<ID3DBlob> rootSigBlob;

	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, nullptr);
	if (FAILED(result)) { assert(0); }

	//���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&graphicsPipeline[_pepelineName].rootSignature));
	if (FAILED(result)) { assert(0); }

}

GraphicsPipelineManager::GraphicsPipelineManager()
{
	//�V�F�[�_�[�̃R���p�C��
	shaderManager = ShaderManager::Create();
}

GraphicsPipelineManager::~GraphicsPipelineManager()
{
	for (auto& pipeline : graphicsPipeline)
	{
		pipeline.second.pipelineState.Reset();
		pipeline.second.rootSignature.Reset();
	}
	shaderManager.reset();
}

void GraphicsPipelineManager::CreatePipeline(const std::string& _name, const PEPELINE& _pepeline, GraphicsPipelineManager::DrawSet& _drawSet)
{
	HRESULT result = S_FALSE;

	std::string pepelineName;
	if (_name == "BasePostEffect") {
		pepelineName = _pepeline.name;
	} else {
		pepelineName = _name + _pepeline.blendMode;
	}
	//�����L�[������΃G���[���o��
	assert(!graphicsPipeline.count(_name));
	size_t L_size = graphicsPipeline.size() + 1;
	graphicsPipeline.reserve(L_size);

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC L_gpipeline = CreatepelineDesc(_pepeline);

	//���[�g�V�O�l�`���̐���
	CreateRootSignature(pepelineName,_pepeline);

	//�p�C�v���C���f�X�N�Ƀ��[�g�V�O�l�`����o�^
	L_gpipeline.pRootSignature = graphicsPipeline[pepelineName].rootSignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&L_gpipeline, IID_PPV_ARGS(&graphicsPipeline[pepelineName].pipelineState));
	if (FAILED(result)) { assert(0); }

	graphicsPipeline[pepelineName].pipelineState->SetName(GetName(pepelineName, "PipelineState"));
	graphicsPipeline[pepelineName].rootSignature->SetName(GetName(pepelineName, "RootSignature"));

	_drawSet.pipeName= pepelineName;
	if (_pepeline.drawType == "POINT") {
		_drawSet.drawType = DrawType::POINT;
	}else if (_pepeline.drawType == "LINE_LIST") {
		_drawSet.drawType = DrawType::LINE_LIST;
	} else if (_pepeline.drawType == "LINE_STRIP") {
		_drawSet.drawType = DrawType::LINE_STRIP;
	} else if (_pepeline.drawType == "TRIANGLE_LIST") {
		_drawSet.drawType = DrawType::TRIANGLE_LIST;
	} else if (_pepeline.drawType == "TRIANGLE_STRIP") {
		_drawSet.drawType = DrawType::TRIANGLE_STRIP;
	}
	_drawSet.constBuffNum = _pepeline.rootparams;
	_drawSet.texNum = _pepeline.textureNum;
}

void GraphicsPipelineManager::PreDraw(ID3D12GraphicsCommandList* _cmdList,const DrawSet& _drawSet)
{
	if (_drawSet.drawType == oldDraw.drawType && _drawSet.pipeName == oldDraw.pipeName) {return;}

	oldDraw = _drawSet;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	_cmdList->SetPipelineState(graphicsPipeline[_drawSet.pipeName].pipelineState.Get());

	// ���[�g�V�O�l�`���̐ݒ�
	_cmdList->SetGraphicsRootSignature(graphicsPipeline[_drawSet.pipeName].rootSignature.Get());

	// �v���~�e�B�u�`���ݒ�
	if (_drawSet.drawType == DrawType::POINT) {
		_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	}else if (_drawSet.drawType == DrawType::LINE_LIST) {
		_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	} else if (_drawSet.drawType == DrawType::LINE_STRIP) {
		_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	} else if (_drawSet.drawType == DrawType::TRIANGLE_LIST) {
		_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	} else if (_drawSet.drawType == DrawType::TRIANGLE_STRIP) {
		_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

}

void GraphicsPipelineManager::ResetDrawSet()
{
	oldDraw.pipeName="null";
}