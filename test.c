#include "minishell.h"

int is_operator(const char *c, int i)
{
    return (c[i] == '|' || c[i] == '>' || c[i] == '<' ||
    (c[i] == '&' && c[i + 1] == '&') || c[i] == '$' || c[i] == '*' || c[i] == '(' || c[i] == ')');
}

int	get_next_qout(char *cmd_line, int i)
{
	int		j;
	char	c;

	j = i + 1;
	c = cmd_line[i];
	while (cmd_line[j])
	{
		if (cmd_line[j] == c)
			return (j);
		j++;
	}
	return (i);
}

int	count_words(char const *s)
{
	int	i;
	int count;
    int q;
    int in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			q = get_next_qout((char *)s, i);
			if (q > i)
				i = q;
			in_word = 1;
		}
        if (is_operator(s, i))
        {
            count++;
            if (is_operator(s, i + 1))
                i++;
            if (!is_operator(s, i + 1) && (s[i + 1] != ' ' || s[i + 1] != '\t') &&
            (s[i + 1] != '\'' || s[i + 1] != '"'))
                in_word = 1;
        }
		if (s[i] == ' ' || s[i] == '\t' || is_operator(s, i))
		{
			if (in_word)
			{
				count++;
				in_word = 0;
			}
		}
		else
			in_word = 1;
		i++;
	}
	if (in_word)
		count++;
	return (count);
}

int main(void)
{
	int i = count_words("echo hello||wc -c");
	printf("[%d]\n", i);
	return (0);
}