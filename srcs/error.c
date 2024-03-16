/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:27:01 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 18:38:26 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 
# include "../libft/libft.h"

extern t_status status;

void	parse_error(t_token *tok, t_token **tol_list)
{
	status.had_error = true;
	write(STDERR_FILENO, "minishell: syntax error near unexpected token `", 47);
	write(STDERR_FILENO, tok->word, ft_strlen(tok->word));
	write(STDERR_FILENO, "'\n", 2);
	while (tok && tok->kind != TK_EOF)
	{
		tok = tok->next;
	}
	*tol_list = tok;
}

void	tokenize_error(char *line, char **line_loc)
{
	extern t_status	status;

	status.had_error = true;
	write(STDERR_FILENO, "minishell: Tokenize Error: ", 27);
	while (*line)
		line++;
	*line_loc = line;
	
}

void	perror_prefix(void)
{
	write(STDERR_FILENO, "minishell: ", 11);
}

void	fatal_error(const char *msg)
{
	perror_prefix();
	write(STDERR_FILENO, "Fatal Error: ", 13);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	exit(-1);
}

void	assert_error(const char *msg)
{
	perror_prefix();
	write(STDERR_FILENO, "Assert Error: ", 14);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	exit(-1);
}
