
__kernel void copyTexture(
  __read_only image2d_t srcTexture,
  __write_only image2d_t dstTexture,
  sampler_t sampler
)
{
    int2 coord = (int2)(get_global_id(0), get_global_id(1));
    write_imagef(dstTexture, coord, read_imagef(srcTexture, sampler, coord));
}