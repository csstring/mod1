float smootherstep(float x);

float smootherstep(float x)
{
  x = clamp((x - 0.0f) / (1.0f - 0.0f), 0.0f, 1.0f);
  return x * x * x * (3 * x * (2 * x - 5) + 10.0f);
}
__kernel void sourcing(
    __read_only image2d_t velocityTemp,
    __read_only image2d_t densityTemp,
    __write_only image2d_t velocity,
    __write_only image2d_t density,
    float4 sourcingVelocity,
    float4 sourcingDensity,
    sampler_t samplerLinearWrap,
    int x,
    int y,
    __global float *A,
    __global float *B,
    __global float *C,
    __global float *D
)
{
  int width = get_image_width(velocityTemp);
  int h = get_image_height(velocityTemp);
  int2 dtID = (int2)(get_global_id(0), get_global_id(1));
  A[dtID.x + width * dtID.y ] = (float)(width);
  B[dtID.x + width * dtID.y ] = (float)(dtID.x);
  C[dtID.x + width * dtID.y ] = (float)(dtID.y);
  D[dtID.x + width * dtID.y ] = (float)(width);
  float radius = 50;
  if(x < width && x > 0 && y < h && y > 0)
  {
    float dist = length(convert_float2(dtID) - (float2)(x,y)) / radius;
    float scale = smootherstep(1.0f - dist);
    write_imagef(velocity, dtID, sourcingVelocity * scale + read_imagef(velocityTemp, samplerLinearWrap, dtID));
    write_imagef(density, dtID, sourcingDensity * scale + read_imagef(densityTemp, samplerLinearWrap, dtID)); 
  }
}