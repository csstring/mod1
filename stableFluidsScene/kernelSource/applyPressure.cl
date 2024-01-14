
__kernel void applyPressure(
    __write_only image2d_t velocity,
    __read_only image2d_t velocityTemp,
    __read_only image2d_t pressureTemp
)
{
  int width = get_image_width(velocity);
  int height = get_image_height(velocity);
  int2 dtID = (int2)(get_global_id(0), get_global_id(1));

  float left = read_imagef(pressureTemp, (int2)(dtID.x == 0 ? width - 1 : dtID.x - 1, dtID.y)).x;
  float right = read_imagef(pressureTemp, (int2)(dtID.x == width - 1 ? 0 : dtID.x + 1, dtID.y)).x;
  float up = read_imagef(pressureTemp, (int2)(dtID.x, dtID.y == height - 1 ? 0 : dtID.y + 1)).x;
  float down = read_imagef(pressureTemp, (int2)(dtID.x, dtID.y == 0 ? height - 1 : dtID.y - 1)).x;
  
  float4 dp = (float4)(right - left, up - down, 0.0f, 0.0f) * 0.5f ;

  write_imagef(velocity, dtID, read_imagef(velocityTemp, dtID) - dp);
}
