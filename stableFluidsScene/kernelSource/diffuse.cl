
__kernel void diffuse(
    __read_only image2d_t velocityTemp,
    __read_only image2d_t densityTemp,
    __write_only image2d_t velocity,
    __write_only image2d_t density,
    float viscosity,
    float dt
)
{
  int width = get_image_width(velocity);
  int height = get_image_height(velocity);
  int2 dtID = (int2)(get_global_id(0), get_global_id(1));

  float4 left = read_imagef(velocityTemp, (int2)(dtID.x == 0 ? width - 1 : dtID.x - 1, dtID.y));
  float4 right = read_imagef(velocityTemp, (int2)(dtID.x == width - 1 ? 0 : dtID.x + 1, dtID.y));
  float4 up = read_imagef(velocityTemp, (int2)(dtID.x, dtID.y == height - 1 ? 0 : dtID.y + 1));
  float4 down = read_imagef(velocityTemp, (int2)(dtID.x, dtID.y == 0 ? height - 1 : dtID.y - 1));
  float4 color = (read_imagef(velocityTemp, dtID) + dt * viscosity * (left + right + up + down)) / (1.0f + 4.0f * viscosity * dt); 
  write_imagef(velocity, dtID, color);

  left = read_imagef(densityTemp, (int2)(dtID.x == 0 ? width - 1 : dtID.x - 1, dtID.y));
  right = read_imagef(densityTemp, (int2)(dtID.x == width - 1 ? 0 : dtID.x + 1, dtID.y));
  up = read_imagef(densityTemp, (int2)(dtID.x, dtID.y == height - 1 ? 0 : dtID.y + 1));
  down = read_imagef(densityTemp, (int2)(dtID.x, dtID.y == 0 ? height - 1 : dtID.y - 1));
  color = (read_imagef(densityTemp, dtID) + dt * viscosity * (left + right + up + down)) / (1.0f + 4.0f * viscosity * dt); 
  write_imagef(density, dtID, color);
} 
