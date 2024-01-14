
__kernel void jacobi(
    __read_only image2d_t divergenceTemp,
    __write_only image2d_t pressure,
    __read_only image2d_t pressureTemp
)
{
  int width = get_image_width(pressure);
  int height = get_image_height(pressure);
  int2 dtID = (int2)(get_global_id(0), get_global_id(1));

  if (dtID.x == 0 && dtID.y == 0)
  {
    write_imagef(pressure, dtID, float4(0.0f));
    return;
  }

  float left = read_imagef(pressureTemp, (int2)(dtID.x == 0 ? width - 1 : dtID.x - 1, dtID.y)).x;
  float right = read_imagef(pressureTemp, (int2)(dtID.x == width - 1 ? 0 : dtID.x + 1, dtID.y)).x;
  float up = read_imagef(pressureTemp, (int2)(dtID.x, dtID.y == height - 1 ? 0 : dtID.y + 1)).x;
  float down = read_imagef(pressureTemp, (int2)(dtID.x, dtID.y == 0 ? height - 1 : dtID.y - 1)).x;
  float div = read_imagef(divergenceTemp, dtID).x;

  float4 ps = float4(0.0f);
  ps.x = 0.25f * (left + right + up + down - div);
  write_imagef(pressure, dtID, ps);
}
