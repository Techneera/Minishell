#include "libft.h"

char	*ft_strdup(const char *s)
{
	char			*ptr;
	char			*start_ptr;
	unsigned int	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s) + 1;
	ptr = (char *) malloc(sizeof(char) * len);
	if (!ptr)
		return (NULL);
	start_ptr = ptr;
	while (*s)
	{
		*ptr = *s;
		ptr++;
		s++;
	}
	*ptr = '\0';
	return (start_ptr);
}
