#include "Globals.h"

#include "PCG/include/pcg_variants.h"
#include "PCG/extras/entropy.h"

pcg32_random_t rng;

uint generateUID()
{
	return pcg32_random_r(&rng);
}