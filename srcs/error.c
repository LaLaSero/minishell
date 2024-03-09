/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:27:01 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/09 19:35:36 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 

extern t_status status;

void parse_error(t_token *tok, t_token **tol_list)
{
	status.had_error = true;

	// error_prefix();
	printf("parse_error: %s\n", tok->word);
	while (tok && tok->kind != TK_EOF)
	{
		// printf("%s\n", tok->word);
		tok = tok->next;
	}
	*tol_list = tok;
}

void	tokenize_error(char *line, char **line_loc)
{
	size_t	i;
	extern t_status	status;

	status.had_error = true;
	// perror_prefix();
	dprintf(STDERR_FILENO, "Tokenize Error: ");
	while (*line)
		line++;
	*line_loc = line;
	
}

void	perror_prefix(void)
{
	dprintf(STDERR_FILENO, "%s", ERROR_PREFIX);
}

void	fatal_error(const char *msg)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
	exit(1);
}

void	assert_error(const char *msg)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "Assert Error: %s\n", msg);
	exit(255);
}
