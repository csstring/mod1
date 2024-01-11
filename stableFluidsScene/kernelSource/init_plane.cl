

typedef struct {
    float4 _position;
    float4 _velocity;
    float _life;   
} Particle;

float rand_lcg(uint *seed);

float rand_lcg(uint *seed) {
    const uint a = 1664525;
    const uint c = 1013904223;
    *seed = a * (*seed) + c;
    return ((float)(*seed) / (float)UINT_MAX);
}

__kernel void init_plane(__global Particle* particles, uint seed) {
    int i = get_global_id(0);
    seed += i;

    float theta = rand_lcg(&seed) * 3.0f - 1.5f;
    float theta2 = rand_lcg(&seed) * 3.0f - 1.5f;
    particles[i]._life = 1.0f;
    particles[i]._position = (float4)(theta, theta2, 0.0f, 1.0f);
    particles[i]._velocity = (float4)(0.0f);
}