
__kernel void printTexture(
  __read_only image2d_t srcTexture,
  int width, int height,
  sampler_t sampler,
  __global float *A,
  __global float *B,
  __global float *C,
  __global float *D
)
{
    int2 coord = (int2)(get_global_id(0), get_global_id(1));
    if (coord.x < width && coord.y < height) 
    {
        float4 pixel = read_imagef(srcTexture, sampler, coord);
        A[coord.x + width * coord.y] = pixel.x;
        B[coord.x + width * coord.y] = pixel.y;
        C[coord.x + width * coord.y] = pixel.z;
        D[coord.x + width * coord.y] = pixel.w;
    }
}