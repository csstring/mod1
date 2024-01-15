
__kernel void computeVorticity(
    __write_only image2d_t vorticity,
    __read_only image2d_t velocity
)
{
  int width = get_image_width(velocity);
  int height = get_image_height(velocity);
  int2 dtID = (int2)(get_global_id(0), get_global_id(1));
  float2 dx = (float2)(1.0f / width, 1.0f/ height);

  float left = read_imagef(velocity, (int2)(dtID.x == 0 ? width - 1 : dtID.x - 1, dtID.y)).y;
  float right = read_imagef(velocity, (int2)(dtID.x == width - 1 ? 0 : dtID.x + 1, dtID.y)).y;
  float up = read_imagef(velocity, (int2)(dtID.x, dtID.y == height - 1 ? 0 : dtID.y + 1)).x;
  float down = read_imagef(velocity, (int2)(dtID.x, dtID.y == 0 ? height - 1 : dtID.y - 1)).x;
  
  float4 vel = (float4)((right - left) / (2.0f * dx.x) - (up - down) / (2.0f * dx.y), 0.0f, 0.0f, 0.0f);

  write_imagef(vorticity, dtID, vel);
}
