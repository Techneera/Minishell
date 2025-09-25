#include "libshell.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*internal_ptr = NULL;
	char		*token_start;

	if (str != NULL)
		internal_ptr = str;
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
