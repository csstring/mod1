
__kernel void advect(
    __read_only image2d_t velocityTemp,
    __read_only image2d_t densityTemp,
    __write_only image2d_t velocity,
    __write_only image2d_t density,
    sampler_t samplerLinearWrap,
    float dt
)
{
    int width = get_image_width(velocityTemp);
    int height = get_image_height(velocityTemp);
    float2 dx = (float2)(1.0f / width, 1.0f / height);
    
    int2 dtID = (int2)(get_global_id(0), get_global_id(1));
    float2 pos = convert_float2(dtID) + 0.5f * dx;

    float2 vel = read_imagef(velocityTemp, samplerLinearWrap, pos).xy;

    float2 posBack = pos - vel * dt;

    write_imagef(velocity, dtID, read_imagef(velocityTemp, samplerLinearWrap, posBack));
    write_imagef(density, dtID, read_imagef(densityTemp, samplerLinearWrap, posBack));
}