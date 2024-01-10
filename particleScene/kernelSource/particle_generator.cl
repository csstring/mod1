

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

__kernel void particle_generator(__global Particle* particles, uint seed, float4 gravity, float dt) {
    int i = get_global_id(0);

    particles[i]._life -= dt;

    if (particles[i]._life >= 0.0f)
        return;
        
        seed += i;
        float speedX = rand_lcg(&seed) * 4.0f - 2.0f;
        float speedY = rand_lcg(&seed) * 4.0f - 2.0f;
        float speedZ = rand_lcg(&seed) * 4.0f - 2.0f;
        particles[i]._position = gravity;
        particles[i]._position.w = 1;
        particles[i]._life = rand_lcg(&seed) * 2.0f;
        particles[i]._velocity = (float4)(speedX, speedY, speedZ, 0.0f);
}