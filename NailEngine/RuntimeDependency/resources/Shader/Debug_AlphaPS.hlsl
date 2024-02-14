#include "Buffers.hlsli"

struct PixelIn
{
    float4 posH : SV_Position;
    float4 posV : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 normalV : NORMAL;
    float3 tangentV : TANGENT;
    float3 biNormalV : BINORMAL;
};

float4 main(PixelIn input) : SV_TARGET
{
    //float4 albedo = AlbedoMap.Sample(sam, input.uv);
    //return input.color * materialColor;
    return materialColor;
    //return input.color * albedo;
}

// ShaderInfo
// ShaderType : Forward
// RasterType : Solid
// CullType : CullNone
// DepthType : LessNoWrite
// BlendType : AlphaBlend
// Sampler : Default