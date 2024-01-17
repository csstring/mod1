// #pragma OPENCL EXTENSION cl_khr_3d_image_writes : enable

float BeerLambert(float absorptionCoefficient, float distanceTraveled);
float3 GetUVW(float3 posModel);
float LightRay(float3 posModel, float3 lightDir, __read_only image3d_t density);

constant float lightAbsorptionCoeff = 5.0f;
constant float densityAbsorption = 10.0;

float BeerLambert(float absorptionCoefficient, float distanceTraveled)
{
    return exp(-absorptionCoefficient * distanceTraveled);
}

float3 GetUVW(float3 posModel)
{
    return (posModel.xyz + 1.0f) * 0.5f;
}

__kernel void cloudLighting(
    __read_only image3d_t density,
    __write_only image3d_t lightMap,
    float3 lightDir,
    sampler_t linearWrapSS
)
{
    int width = get_image_width(density);
    int height = get_image_height(density);
    int depth = get_image_depth(density);
    int3 dtID = (int3)(get_global_id(0), get_global_id(1), get_global_id(2));

    float3 uvw = convert_float3(dtID) / (float3)(width, height, depth); //+uvwoffset 구름 이동인듯?
    float3 posModel = (uvw - 0.5f) * 2.0f;

    int numSteps = 128 / 4;
    float stepSize = 2.0f / convert_float(numSteps);

    float alpha = 1.0f;

    for (int i = 0; i < numSteps; i++)
    {
        // float prevAlpha = alpha;
        float densityVal = read_imagef(density, linearWrapSS, (float4)(GetUVW(posModel),0.0f)).x; 
        
        if (densityVal > 1e-3)
            alpha *= BeerLambert(lightAbsorptionCoeff * densityVal, stepSize);

        posModel += lightDir * stepSize;

        if (fabs(posModel.x) > 1 || fabs(posModel.y) > 1 || fabs(posModel.z) > 1)
            break;
        
        if (alpha < 1e-3)
            break;
    }
    
    write_imagef(lightMap, (int4)(dtID,0), (float4)(alpha, 0.0f,0.0f,0.0f));
}
