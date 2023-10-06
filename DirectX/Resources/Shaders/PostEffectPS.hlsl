#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> bloom : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> outline : register(t2); //2�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> fog : register(t3); //2�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float> depthTex : register(t4); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//���C���J���[
	float4 mainColor = tex.Sample(smp, input.uv);
	
	//bloom
	float4 bloomColor = bloom.Sample(smp, input.uv);

	//bloom
	float4 outlineColor = outline.Sample(smp, input.uv);
	
	//fog
	float4 fogColor = fog.Sample(smp, input.uv);

	//�|�X�g�G�t�F�N�g�̍���
    return mainColor + bloomColor + outlineColor + fogColor;
}