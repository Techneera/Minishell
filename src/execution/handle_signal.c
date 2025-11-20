#include "execution.h"

void	handle_sigstop(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	write(1, PROMPT, ft_strlen(PROMPT));
}

void	handle_sigstop_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}
