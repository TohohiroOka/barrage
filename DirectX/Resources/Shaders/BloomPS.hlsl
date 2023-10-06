#include "Bloom.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

/// <summary>
/// �K�E�X����
/// </summary>
float Gaussian(float2 drawUV, float2 pickUV, float sigma);

float4 main(VSOutput input) : SV_TARGET
{
    float totalWeight = 0;
    float sigma = 0.005;
    float stepWidth = 0.001;
    float4 color = { 0, 0, 0, 0 };

    for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
    {
        for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float add1 = (pickUV.x <= 1) * (pickUV.x >= 0);
            float add2 = (pickUV.y <= 1) * (pickUV.y >= 0);
            float weight = Gaussian(input.uv, pickUV, sigma);
            color += tex.Sample(smp, pickUV) * weight * add1 * add2;
            totalWeight += weight;
        }
    }

    color.rgb = color.rgb / totalWeight;

	//���ȉ��؂�̂�
    return color;
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-d * d) / (2 * sigma * sigma);
}