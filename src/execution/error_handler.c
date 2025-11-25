#include "execution.h"

void	failed_malloc(t_data *data, char *str)
{
	perror(str, STDERR_FILENO);
	free_data(data);
	exit(FAIL_STATUS);
}
