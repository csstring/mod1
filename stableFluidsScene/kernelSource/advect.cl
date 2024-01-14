
__kernel void advect(
    __read_only image2d_t velocityTemp,
    __read_only image2d_t densityTemp,
    __write_only image2d_t velocity,
    __write_only image2d_t density,
    sampler_t pointWrapSS,
    sampler_t linearWrapSS,
    float dt,
    __global float *A,
    __global float *B,
    __global float *C,
    __global float *D
)
{
    int width = get_image_width(velocityTemp);
    int height = get_image_height(velocityTemp);
    // float2 dx = (float2)(1.0f / width, 1.0f / height);
    
    int2 dtID = (int2)(get_global_id(0), get_global_id(1));
    float2 pos = (convert_float2(dtID) + float2(0.5f));
    // A[dtID.x + width * dtID.y ] = pos.x;
    // B[dtID.x + width * dtID.y ] = pos.y;
    // C[dtID.x + width * dtID.y ] = dx.x;
    // D[dtID.x + width * dtID.y ] = dx.y;
    float2 vel = read_imagef(velocityTemp, pointWrapSS, pos).xy;
    // float2 veltmp = (float2)(1.0,0.0);
    float2 posBack = pos - vel * dt * 10.0f;
    // float2 posBack = pos;
    write_imagef(velocity, dtID, read_imagef(velocityTemp, linearWrapSS, posBack));
    write_imagef(density, dtID, read_imagef(densityTemp, linearWrapSS, posBack));
}
