#include "minishell.h"

typedef struct s_tok
{
	char			*line;
	char			*treated_line;
	struct s_tok	*next;
}					t_tok;

typedef struct s_node
{
	char			*line;
	char			*treated_line;
	struct s_node	*next;
}					t_node;

char	*remove_single_quote(char *line)
{
	char	*start;
	int		len;

	while (*line != '\0')
	{
		if (*line == '\'')
		{
			line++;
			start = line;
			len = 0;
			while (*line != '\'')
			{
				if (*line == '\0')
					printf("===error====\n");
				line++;
				len++;
			}
		}
		line++;
	}
	return (ft_substr(start, 0, len));
}

char	*expand_variable(char *line)
{
	int		i;
	char	*variable_name;
	char	*result;
	char	*start;

	while (*line != '\0')
	{
		if (*line == '$')
		{
			start = line;
			line++;
			i = 0;
			while (*line != '\0' && *line != ' ')
			{
				line++;
				i++;
			}
			variable_name = ft_substr(start, 0, i);
			// search env
		}
		line++;
	}
	return (variable_name);
	// return (result);
}

char	*expand_status()
{
	char	*result;

	result = ft_itoa(status);
	if (result == NULL)
		

}

char	*remove_double_quote(char *line)
{
	char	*start;
	char	*result;
	int		len;

	while (*line != '\0')
	{
		if (*line == '\"')
		{
			line++;
			start = line;
			len = 0;
			while (*line != '\"')
			{
				if (*line == '\0')
					printf("===error====\n");
				line++;
				len++;
			}
		}
		line++;
	}
	result = ft_substr(start, 0, len);
	return (result);
}

t_tok	*expand(t_tok *ptr)
{
	if (!isword(ptr->line))
		return (NULL);
	if (*(ptr->line) == '\'')
		ptr->treated_line = remove_single_quote(ptr->line);
	else if (*(ptr->line) == '\"')
		ptr->treated_line = remove_double_quote(ptr->line);
	else if (*(ptr->line) == '$')
	{
		if (*((ptr->line) + 1) == '\?')
			ptr->treated_line = expand_status();
		else
			ptr->treated_line = expand_variable(ptr->line);
	}
	expand(ptr->next);
	return (ptr);
}

int	main(void)
{
	char	*result;
	char	*result2;
	// テスト用の文字列
	char input[] = "This is 'a test' string.";
	char input2[] = "This is \"a test\" string.";
	// 関数を呼び出し
	result = remove_single_quote(input);
	result2 = remove_double_quote(input2);
	// 結果を表示
	printf("Original: %s\n", input);
	printf("Original: %s\n", input2);
	printf("Modified: %s\n", result);
	printf("Modified: %s\n", result2);
	free(result);
	free(result2);
	return (0);
}
