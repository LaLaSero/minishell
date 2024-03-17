/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:19 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 01:25:46 by yutakagi         ###   ########.fr       */
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
			p++;
			while (*p != SINGLE_QUOTE_CHAR)
			{
				if (*p == '\0')
					assert_error("Unclosed single quote");
				append_char(&new_word, *p++);
			}
			p++;
		}
		else if (*p == DOUBLE_QUOTE_CHAR)
		{
			p++;
			while (*p != DOUBLE_QUOTE_CHAR)
			{
				if (*p == '\0')
					assert_error("Unclosed double quote");
				append_char(&new_word, *p++);
			}
			p++;
		}
		else
			append_char(&new_word, *p++);
	}
	free(tok->word);
	tok->word = new_word;
	quote_removal(tok->next);
}

// 何も展開せずにdstにシングルクオートを含む文字列をappendする
void	append_single_quote(char **dst, char **rest, char *p)
{
	append_char(dst, *p++);
	while (*p != SINGLE_QUOTE_CHAR)
	{
		if (*p == '\0')
			assert_error("Unclosed single quote");
		append_char(dst, *p++);
	}
	append_char(dst, *p++);
	*rest = p;
}

// ダブルクオートの中の$variableは展開した上で、dstにダブルクオートを含む文字列をappendする
void	append_double_quote(char **dst, char **rest, char *p)
{
	append_char(dst, *p++);
	while (*p != DOUBLE_QUOTE_CHAR)
	{
		if (*p == '\0')
			assert_error("Unclosed double quote");
		else if (is_variable(p))
			expand_variable_str(dst, &p, p);
		else
			append_char(dst, *p++);
	}
	append_char(dst, *p++);
	*rest = p;
}