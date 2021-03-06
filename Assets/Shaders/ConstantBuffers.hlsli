#include"Light.hlsli"

cbuffer Instance : register(b0)
{
    matrix World;
    
    uint MaterialID;
    uint3 InstancePadding;
    
};

cbuffer Transform : register(b1)
{
    matrix View;
    matrix Projection;
    float4 ViewPosition;
};

cbuffer Scene : register(b2)
{
    DirectionalLight DirectionalLights[1024];
    SpotLight SpotLights[1024];
    
    uint DirectionalCount;
    uint SpotCount;
    uint2 ScenePadding;
};