cbuffer cbuff0 : register(b0)
{
    float strength; //bloom�̋���
    float3 color;//fog�̐F
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};