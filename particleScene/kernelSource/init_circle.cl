

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

__kernel void init_circle(__global Particle* particles, uint seed) {
    int i = get_global_id(0);
    seed += i;
    float theta = rand_lcg(&seed) * 2.0f * 3.141592f - 3.141592f;
    float speedX = rand_lcg(&seed) * 4.0f - 2.0f;
    float speedY = rand_lcg(&seed) * 4.0f - 2.0f;
    float speedZ = rand_lcg(&seed) * 4.0f - 2.0f;

    // Set particle position on the edge of a circle
    particles[i]._position = (float4)(cos(theta), -sin(theta), 0.0f, 1.0f);
    particles[i]._velocity = (float4)(speedX, speedY, speedZ, 0.0f);
    particles[i]._life = 1.0f;
}