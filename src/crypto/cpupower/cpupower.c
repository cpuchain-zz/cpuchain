#include "cpupower.h"

int cpupower_hash(const char *input, char *output)
{
	yespower_params_t params = {
		.version = YESPOWER_1_0,
		.N = 2048,
		.r = 32,
		.pers = "CPUpower: The number of CPU working or available for proof-of-work mining",
		.perslen = 73
  };
	return yespower_tls(input, 80, &params, (yespower_binary_t *) output);
}
