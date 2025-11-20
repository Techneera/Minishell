#include "libshell.h"

int	ft_exit_status(int state, int write_, int exit_)
{
	static int	current_status;

	if (write_ != 0)
		current_status = 1;
}
