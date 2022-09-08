// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such perf, many querry!
// (this looks complicated...)
//___________________________________________________________________________________________________________________________________________

#include <inttypes.h>

void aggregate_4bit(uint32_t * dest, uint32_t val)
{
	if ((((*dest) + val) < (*dest)) || ((*dest) + val) > 0xf)
		(*dest) = 0xf;
	else
		(*dest) = (*dest) + val;
}

void aggregate_8bit(uint32_t * dest, uint32_t val)
{
	if ((((*dest) + val) < (*dest))
	    || ((*dest) + val) > 0xff)
		(*dest) = 0xff;
	else
		(*dest) = (*dest) + val;
}

void aggregate_16bit(uint32_t * dest, uint32_t val)
{
	if ((((*dest) + val) < (*dest))
	    || ((*dest) + val) > 0xffff)
		(*dest) = 0xffff;
	else
		(*dest) = (*dest) + val;
}

void aggregate_32bit(uint32_t * dest, uint32_t val)
{
	if (((*dest) + val) < (*dest))
		(*dest) = 0xffffffff;
	else
		(*dest) = (*dest) + val;
}

void aggregate_64bit(uint64_t * dest, uint64_t val)
{
	if (((*dest) + val) < (*dest))
		(*dest) = 0xffffffffffffffffULL;
	else
		(*dest) = (*dest) + val;
}