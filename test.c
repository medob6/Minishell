#include "minishell.h"

int is_operator(const char *c, int i)
{
	return ((c[i] == '&' && c[i + 1] == '&') || (c[i] == '|' && c[i + 1] == '|') || c[i] == '|' || c[i] == '<' || c[i] == '>' || c[i] == '(' || c[i] == ')');
}

int skip_quotes(const char *s, int i, int *in_word)
{
	char c;

	if (strchr("'\"", s[i]) != NULL)
	{
		c = s[i];
		i++;
		while (s[i] && s[i] != c)
			i++;
		if (s[i] == c)
			i++;
		*in_word = 1;
	}
	return (i);
}

size_t ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t count;
	size_t i;

	count = strlen(src);
	i = 0;
	if (dstsize != 0)
	{
		while ((src[i]) && (i < dstsize - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (count);
}

int handle_word(char **str, char *cmd_line, int *i)
{
	int start;
	int n = 0;
	char quote;

	start = *i;

	if (cmd_line[*i] == '\'' || cmd_line[*i] == '"')
	{
		quote = cmd_line[*i];
		start = *i;
		while (cmd_line[*i] == '\'' || cmd_line[*i] == '"')
			*i += skip_quotes(cmd_line, *i, &n);
	}
	if (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t' && !is_operator(cmd_line, *i))
	{
		while (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t' && !is_operator(cmd_line, *i))
			(*i)++;
	}

	*str = malloc(*i - start + 1);
	ft_strlcpy(*str, &cmd_line[start], *i - start + 1);
	return (1);
}

int main()
{
    char **str = malloc(sizeof(char *) * (2));
    int     i = 0;
    char *s = "\"e\"e";
    int     j = 0;
    while (s[i])
    {
        printf("here\n");
        j += handle_word(&str[j], s, &i);
    }
    return (0);
}