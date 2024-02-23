
__kernel void confineVorticity(
    __read_only image2d_t vorticity,
    __read_only image2d_t velocitytemp,
    __write_only image2d_t velocity,
    float dt
)
{
  int width = get_image_width(velocity);
  int height = get_image_height(velocity);
  int2 dtID = (int2)(get_global_id(0), get_global_id(1));
  float2 dx = (float2)(1.0f / width, 1.0f/ height);

  float left = fabs(read_imagef(vorticity, (int2)(dtID.x == 0 ? width - 1 : dtID.x - 1, dtID.y)).x);
  float right = fabs(read_imagef(vorticity, (int2)(dtID.x == width - 1 ? 0 : dtID.x + 1, dtID.y)).x);
  float up = fabs(read_imagef(vorticity, (int2)(dtID.x, dtID.y == height - 1 ? 0 : dtID.y + 1)).x);
  float down = fabs(read_imagef(vorticity, (int2)(dtID.x, dtID.y == 0 ? height - 1 : dtID.y - 1)).x);
  
  float2 eta = (float2)((right - left) / (2.0f * dx.x), -(up - down) / (2.0f * dx.y));
  if (length(eta) < 1e-5){
    return;
  }
  float3 psi = (float3)(normalize(eta).xy, 0.0f);
  float3 omega = (float3)(0.0f, 0.0f, read_imagef(vorticity, dtID).x);
  float4 res = (float4)(cross(psi, omega).xy * 0.02f * dt, 0.0f,0.0f);
  write_imagef(velocity, dtID, read_imagef(velocitytemp,dtID) + res);
}
