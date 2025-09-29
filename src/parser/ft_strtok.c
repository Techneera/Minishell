#include "libshell.h"

void	define_delim(char *internal_ptr, char delim[2]);

char	*ft_strtok(char *str)
{
	static char	*internal_ptr = NULL;
	char		*token_start;
	char		delim[2];

	if (str != NULL)
		internal_ptr = str;
	define_delim(internal_ptr, delim);
	if (*internal_ptr == '\0')
		return (NULL);
	while (*internal_ptr != '\0' && strchr(delim, *internal_ptr))
		internal_ptr++;
	if (*internal_ptr == '\0')
	{
		internal_ptr = NULL;
		return (NULL);
	}
	token_start = internal_ptr;
	while (*internal_ptr != '\0' && !strchr(delim, *internal_ptr))
		internal_ptr++;
	if (*internal_ptr != '\0')
	{
		*internal_ptr = '\0';
		internal_ptr++;
	}
	return (token_start);
}

void	define_delim(char *internal_ptr, char delim[2])
{
	*delim = ' ';
	if (*internal_ptr == '\'')
		*delim = ('\'');
	if (*internal_ptr == '"')
		*delim = ('"');
}
