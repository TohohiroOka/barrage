cbuffer cbuff0 : register(b0)
{
    float strength; //bloomの強さ
    float blendRate; //カラーの割合
    float3 color1; //fogの色（中央）
    float3 color2; //fogの色（端）
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};