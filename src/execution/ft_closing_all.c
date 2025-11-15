#include "execution.h"

static void	closing_docs(t_fds **fds);
static void	closing_files(t_fds **fds);

void	ft_closing_all(t_fds **fds)
{
	if (!fds || !*fds)
		return ;
	closing_docs(fds);
	closing_files(fds);
}

static void	closing_docs(t_fds **fds)
{
	int	i;

	i = 0;
	while (i < (*fds)->get.n_docs)
	{
		if ((*fds)->heredoc_fds && (*fds)->heredoc_fds[i])
		{
			if ((*fds)->heredoc_fds[i][0] != -1)
			{
				close((*fds)->heredoc_fds[i][0]);
				(*fds)->heredoc_fds[i][0] = -1;
			}
			if ((*fds)->heredoc_fds[i][1] != -1)
			{
				close((*fds)->heredoc_fds[i][1]);
				(*fds)->heredoc_fds[i][1] = -1;
			}
		}
		i++;
	}
}

static void	closing_files(t_fds **fds)
{
	int	i;

	i = 0;
	while (i < (*fds)->get.n_files)
	{
		if ((*fds)->fd_files)
		{
			if ((*fds)->fd_files[i] != -1)
			{
				close((*fds)->fd_files[i]);
				(*fds)->fd_files[i] = -1;
			}
		}
		i++;
	}
}
