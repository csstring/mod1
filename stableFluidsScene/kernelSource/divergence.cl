
__kernel void divergence(
    __write_only image2d_t divergence,
    __read_only image2d_t velocityTemp,
    __write_only image2d_t pressure,
    __write_only image2d_t pressureTemp
)
{
  int width = get_image_width(divergence);
  int height = get_image_height(divergence);
  int2 dtID = (int2)(get_global_id(0), get_global_id(1));

  float left = read_imagef(velocityTemp, (int2)(dtID.x == 0 ? width - 1 : dtID.x - 1, dtID.y)).x;
  float right = read_imagef(velocityTemp, (int2)(dtID.x == width - 1 ? 0 : dtID.x + 1, dtID.y)).x;
  float up = read_imagef(velocityTemp, (int2)(dtID.x, dtID.y == height - 1 ? 0 : dtID.y + 1)).y;
  float down = read_imagef(velocityTemp, (int2)(dtID.x, dtID.y == 0 ? height - 1 : dtID.y - 1)).y;
  float4 div = float4(0.0f);
  div.x = 0.5f*(right - left + up - down);
  write_imagef(divergence, dtID, div);
  write_imagef(pressure, dtID, float4(0.0f));
  write_imagef(pressureTemp, dtID, float4(0.0f));
}
