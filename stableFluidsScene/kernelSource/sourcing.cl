// float smootherstep(float x);

// float smootherstep(float x)
// {
//   x = clamp((x - 0.0f) / (1.0f - 0.0f), 0.0f, 1.0f);
//   return x * x * x * (3 * x * (2 * x - 5) + 10.0f);
// }
const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_FILTER_NEAREST;
kernel void sourcing(
    read_only image2d_t velocity,
    read_write image2d_t density,
    // sampler_t samplerLinearWrap,
    float4 cursor
    // float4 sourcingVelocity,
    // float4 sourcingDensity
)
{
  // size_t x = get_global_id(0);
  // size_t y = get_global_id(1);
  int2 dtID = (int2)(get_global_id(0), get_global_id(1));
  // float4 tmp = (float4)cursor;
  // read_imagef(density, sampler, dtID);
  write_imagef(density, dtID, cursor);
  // int radius = 5;
  // float dist = length(convert_float2(dtID) - cursor.xy) / radius;
  // float scale = smootherstep(1.0f - dist);

  // write_imagef(density, dtID, sourcingVelocity * scale + read_imagef(velocity, samplerLinearWrap, dtID));
  // write_imagef(velocity, dtID, sourcingDensity * scale + read_imagef(velocity, samplerLinearWrap, dtID)); 
}