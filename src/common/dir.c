#include "dir_ext.h"

static enum dir_t map[] = {
	DIR_NW,
	DIR_NORTH,
	DIR_NE,
	DIR_WEST,
	NO_DIR,
	DIR_EAST,
	DIR_SW,
	DIR_SOUTH,
	DIR_SE,
};

unsigned int twoD_offset_to_dir(unsigned int dx, unsigned int dy)
{
	dx += 1;
	dy += 1;
	return map[dx + (dy * 3)];
}