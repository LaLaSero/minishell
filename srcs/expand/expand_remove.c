/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 21:46:05 by kishizu           #+#    #+#             */
/*   Updated: 2024/03/19 13:44:45 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
#include "../libft/libft.h"

void	remove_quote(t_node *node, int *error)
{
	if (node == NULL)
		return ;
	quote_removal(node->args, error);
	quote_removal(node->filename, error);
	quote_removal(node->delimiter, error);
	remove_quote(node->redirect, error);
	remove_quote(node->command, error);
	remove_quote(node->next, error);
}

void	remove_single_quote(char *p, char **p_loc, char **new_word, int *error)
{
	p++;
	while (*p != SINGLE_QUOTE_CHAR)
	{
		if (*p == '\0')
			assert_error("Unclosed single quote", error);
		append_char(new_word, *p++);
	}
	p++;
	*p_loc = p;
}

void	remove_doule_quote(char *p, char **p_loc, char **new_word, int *error)
{
	p++;
	while (*p != DOUBLE_QUOTE_CHAR)
	{
		if (*p == '\0')
			assert_error("Unclosed double quote", error);
		append_char(new_word, *p++);
	}
	p++;
	*p_loc = p;
}

void	quote_removal(t_token *tok, int *error)
{
	char	*new_word;
	char	*p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	if (ft_strncmp(p, "\"\"", 3) == 0 || ft_strncmp(p, "\'\'", 3) == 0)
		new_word = ft_strdup("");
	else
	{
		new_word = NULL;
		while (*p)
		{
			if (*p == SINGLE_QUOTE_CHAR)
				remove_single_quote(p, &p, &new_word, error);
			else if (*p == DOUBLE_QUOTE_CHAR)
				remove_doule_quote(p, &p, &new_word, error);
			else
				append_char(&new_word, *p++);
		}
	}
	free(tok->word);
	tok->word = new_word;
	quote_removal(tok->next, error);
}
