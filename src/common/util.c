#include "util.h"

void _check_error(void *pointer, enum error_type error_type, uint line, const char *file)
{
    switch (error_type)
    {
    case MALLOC_ERR:
        if (pointer == NULL)
        {
            fprintf(stderr, "Error at file %s:%u\nMalloc failed to allocate the needed memory.\n", file, line);
            exit(MALLOC_ERR);
        }
        break;
    case FILE_ERR:

        break;

    default:
        break;
    }
}