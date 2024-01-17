// #pragma OPENCL EXTENSION cl_khr_3d_image_writes : enable

constant uint3 UI3 = (uint3)(1597334673U, 3812015801U, 2798796415U);
constant float UIF = 1.0f / 4294967295.f;
float3 hash33(float3 p);
float remap(float x, float a, float b, float c, float d);
float gradientNoise(float3 x, float freq);
float worleyNoise(float3 uv, float freq);
float perlinfbm(float3 p, float freq, int octaves);
float worleyFbm(float3 p, float freq);
float makeCloudDensity(float3 uvw);

float3 hash33(float3 p)
{
	uint3 q = convert_uint3(convert_int3(p)) * UI3;
	q = (q.x ^ q.y ^ q.z)*UI3;
	return -1.f + 2.f * convert_float3(q) * UIF;
}

float remap(float x, float a, float b, float c, float d)
{
    return (((x - a) / (b - a)) * (d - c)) + c;
}

// Gradient noise by iq (modified to be tileable)
float gradientNoise(float3 x, float freq)
{
    // grid
    float3 p;
    float3 w = fract(x, &p);
    
    // quintic interpolant
    float3 u = w * w * w * (w * (w * 6.f - 15.f) + 10.f);

    
    // gradients
    float3 ga = hash33(fmod(p + (float3)(0., 0., 0.), freq));
    float3 gb = hash33(fmod(p + (float3)(1., 0., 0.), freq));
    float3 gc = hash33(fmod(p + (float3)(0., 1., 0.), freq));
    float3 gd = hash33(fmod(p + (float3)(1., 1., 0.), freq));
    float3 ge = hash33(fmod(p + (float3)(0., 0., 1.), freq));
    float3 gf = hash33(fmod(p + (float3)(1., 0., 1.), freq));
    float3 gg = hash33(fmod(p + (float3)(0., 1., 1.), freq));
    float3 gh = hash33(fmod(p + (float3)(1., 1., 1.), freq));
    
    // projections
    float va = dot(ga, w - (float3)(0., 0., 0.));
    float vb = dot(gb, w - (float3)(1., 0., 0.));
    float vc = dot(gc, w - (float3)(0., 1., 0.));
    float vd = dot(gd, w - (float3)(1., 1., 0.));
    float ve = dot(ge, w - (float3)(0., 0., 1.));
    float vf = dot(gf, w - (float3)(1., 0., 1.));
    float vg = dot(gg, w - (float3)(0., 1., 1.));
    float vh = dot(gh, w - (float3)(1., 1., 1.));
	
    // interpolation
    return va + 
           u.x * (vb - va) + 
           u.y * (vc - va) + 
           u.z * (ve - va) + 
           u.x * u.y * (va - vb - vc + vd) + 
           u.y * u.z * (va - vc - ve + vg) + 
           u.z * u.x * (va - vb - ve + vf) + 
           u.x * u.y * u.z * (-va + vb + vc - vd + ve - vf - vg + vh);
}

// Tileable 3D worley noise
float worleyNoise(float3 uv, float freq)
{    
    float3 id;
    float3 p = fract(uv, &id);
    
    float minDist = 10000.f;
    for (float x = -1.f; x <= 1.f; ++x)
    {
        for(float y = -1.f; y <= 1.f; ++y)
        {
            for(float z = -1.f; z <= 1.f; ++z)
            {
                float3 offset = (float3)(x, y, z);
            	float3 h = hash33(fmod(id + offset, float3(freq))) * .5f + .5f;
    			h += offset;
            	float3 d = p - h;
           		minDist = min(minDist, dot(d, d));
            }
        }
    }
    
    // inverted worley noise
    return 1. - minDist;
}

// Fbm for Perlin noise based on iq's blog
float perlinfbm(float3 p, float freq, int octaves)
{
    float G = exp2(-.85);
    float amp = 1.f;
    float noise = 0.f;
    for (int i = 0; i < octaves; ++i)
    {
        noise += amp * gradientNoise(p * freq, freq);
        freq *= 2.;
        amp *= G;
    }
    
    return noise;
}

float worleyFbm(float3 p, float freq)
{
    return worleyNoise(p*freq, freq) * .625f +
        	 worleyNoise(p*freq*2.f, freq*2.f) * .25f +
        	 worleyNoise(p*freq*4.f, freq*4.f) * .125f;
}

float makeCloudDensity(float3 uvw)
{
    float freq = 4.0f;
    
    float pfbm = mix(1.f, perlinfbm(uvw, 4.f, 7), .5f);
    pfbm = fabs(pfbm * 2.f - 1.f); // billowy perlin noise

    float g = worleyFbm(uvw, freq);
    float r = remap(pfbm, 0.f, 1.f, g, 1.f); // perlin-worley
    float b = worleyFbm(uvw, freq * 2.0f);
    float a = worleyFbm(uvw, freq * 4.0f);

    float perlinWorley = r;
    float wfbm = g * .625f + b * .125f + a * .25f;

    float cloud = remap(perlinWorley, wfbm - 1.f, 1.f, 0.f, 1.f);
    cloud = remap(cloud, .85f, 1.f, 0.f, 1.f); // fake cloud coverage
    
    return clamp(cloud, 0.0f, 1.0f);
}

__kernel void cloudDensity(
    __write_only image3d_t density
)
{
    int width = get_image_width(density);
    int height = get_image_height(density);
    int depth = get_image_depth(density);
    int3 dtID = (int3)(get_global_id(0), get_global_id(1), get_global_id(2));

    float3 uvw = convert_float3(dtID) / (float3)(width, height, depth); //+uvwoffset 구름 이동인듯?

    write_imagef(density, (int4)(dtID,0), (float4)(makeCloudDensity(uvw), 0.0f,0.0f,0.0f));
}
