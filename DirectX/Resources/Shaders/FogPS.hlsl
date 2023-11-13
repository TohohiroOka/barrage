#include "Fog.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float absTex = abs(tex.Sample(smp, input.uv));
    float depth = pow(absTex, 10000);
    depth = depth * depth * strength;
    float3 outColor1 = color1 * depth;
    float3 outColor2 = color2 * depth;
    
    float x = smoothstep(0, 0.5, input.uv.x) * smoothstep(1.0, 0.5, input.uv.x);
    float y = smoothstep(0, 0.5, input.uv.y) * smoothstep(1.0, 0.5, input.uv.y);

    float color1Rate = smoothstep(0, blendRate, x * y);
    float color2Rate = (1 - smoothstep(0, blendRate, x * y));

    return float4(outColor1 * color1Rate + outColor2 * color2Rate, 1.0f * depth);
}