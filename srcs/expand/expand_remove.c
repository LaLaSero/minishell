/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 21:46:05 by kishizu           #+#    #+#             */
/*   Updated: 2024/03/18 21:46:57 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
#include "../libft/libft.h"

void	remove_quote(t_node *node)
{
	if (node == NULL)
		return ;
	quote_removal(node->args);
	quote_removal(node->filename);
	quote_removal(node->delimiter);
	remove_quote(node->redirect);
	remove_quote(node->command);
	remove_quote(node->next);
}

void	remove_single_quote(char *p, char **p_loc, char **new_word)
{
	p++;
	while (*p != SINGLE_QUOTE_CHAR)
	{
		if (*p == '\0')
			assert_error("Unclosed single quote");
		append_char(new_word, *p++);
	}
	p++;
	*p_loc = p;
}

void	remove_doule_quote(char *p, char **p_loc, char **new_word)
{
	p++;
	while (*p != DOUBLE_QUOTE_CHAR)
	{
		if (*p == '\0')
			assert_error("Unclosed double quote");
		append_char(new_word, *p++);
	}
	p++;
	*p_loc = p;
}

void	quote_removal(t_token *tok)
{
	char	*new_word;
	char	*p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = NULL;
	while (*p)
	{
		if (*p == SINGLE_QUOTE_CHAR)
		{
			remove_single_quote(p, &p, &new_word);
		}
		else if (*p == DOUBLE_QUOTE_CHAR)
		{
			remove_doule_quote(p, &p, &new_word);
		}
		else
			append_char(&new_word, *p++);
	}
	free(tok->word);
	tok->word = new_word;
	quote_removal(tok->next);
}