/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 19:11:40 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 19:19:37 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../includes/minishell.h" 

bool	is_op(t_token *tok, char *op)
{
	if (tok->kind != TK_OP)
		return (false);
	return (ft_strncmp(tok->word, op, ft_strlen(tok->word) + 1) == 0);
}

bool	is_control_operator(t_token *tok)
{
	size_t				i;

	static char *const
		operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (ft_strncmp(tok->word, operators[i],
				ft_strlen(operators[i]) + 1) == 0)
			return (true);
		i++;
	}
	return (false);
}

t_token	*tokdup(t_token *tok)
{
	char	*word;

	word = ft_strdup(tok->word);
	if (word == NULL)
		fatal_error("strdup");
	return (new_token(word, tok->kind));
}

t_node	*_the_first_node(t_node_kind kind)
{
	t_node	*node;

	node = new_node(kind);
	node->inpipe[0] = STDIN_FILENO;
	node->inpipe[1] = -1;
	node->outpipe[0] = -1;
	node->outpipe[1] = STDOUT_FILENO;
	return (node);
}
