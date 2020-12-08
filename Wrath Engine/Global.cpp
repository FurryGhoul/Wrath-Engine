#include "Globals.h"

#include "PCG/include/pcg_variants.h"
#include "PCG/extras/entropy.h"
#include <time.h>
#include <math.h>

pcg32_random_t rng;

void StartPCGRNG()
{
	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
}

uint Generate_UUID()
{
	return pcg32_random_r(&rng);
}