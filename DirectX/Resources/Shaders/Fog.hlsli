cbuffer cbuff0 : register(b0)
{
    float strength; //bloom�̋���
    float blendRate; //�J���[�̊���
    float3 color1; //fog�̐F�i�����j
    float3 color2; //fog�̐F�i�[�j
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};