/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/03/17 05:05:31 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
#include "../libft/libft.h"

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += ft_strlen(*s);
	new = malloc(size);
	if (new == NULL)
		fatal_error("malloc");
	if (*s)
		strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

bool	is_alpha_under(char c)
{
	return (isalpha(c) || c == '_');
}

bool	is_alpha_num_under(char c)
{
	return (is_alpha_under(c) || isdigit(c));
}

bool	is_identifier(const char *s)
{
	if (!is_alpha_under(*s))
		return (false);
	s++;
	while (*s)
	{
		if (!is_alpha_num_under(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	is_variable(char *s)
{
	return (s[0] == '$' && is_alpha_under(s[1]));
}

bool is_macro(char *s)
{
	return ((s[0] == '$' && s[1] == '?'));
}

void expand_macro(char **dst, char **rest, char *p)
{
	extern t_status status;
	char *temp;

	if (is_macro(p) == false)
		assert_error("Expected $? macro");
	p += ft_strlen("$?");
	temp = *dst;
	*dst = ft_itoa(status.exit_status);
	free(temp);
	*rest = p;
}

// $variable を展開する
void	expand_variable_str(char **dst, char **rest, char *p)
{
	char	*var_name;
	char	*value;

	var_name = ft_calloc(1, sizeof(char));
	if (var_name == NULL)
		fatal_error("calloc");
	if (*p != '$')
		assert_error("Expected dollar sign");
	p++;
	if (!is_alpha_under(*p))
		assert_error("Variable must starts with alphabetic character or underscore.");
	append_char(&var_name, *p++);
	while (is_alpha_num_under(*p))
		append_char(&var_name, *p++);
	value = get_value(var_name);
	free(var_name);
	if (value)
		while (*value)
			append_char(dst, *value++);
	else
		append_char(dst, '\0');
	*rest = p;
}

// 何も展開せずにdstにシングルクオートを含む文字列をappendする
void	append_single_quote(char **dst, char **rest, char *p)
{
	// skip quote
	append_char(dst, *p++);
	while (*p != SINGLE_QUOTE_CHAR)
	{
		if (*p == '\0')
			assert_error("Unclosed single quote");
		append_char(dst, *p++);
	}
	// skip quote
	append_char(dst, *p++);
	*rest = p;
	// }
	// else
	// 	assert_error("Expected single quote");
}

// ダブルクオートの中の$variableは展開した上で、dstにダブルクオートを含む文字列をappendする
void	append_double_quote(char **dst, char **rest, char *p)
{
		// skip quote
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
		// skip quote
		append_char(dst, *p++);
		*rest = p;
	// else
	// 	assert_error("Expected double quote");
}

// トークンに含まれるvariableを展開する
void	expand_variable_tok(t_token *tok)
{
	char	*new_word;
	char	*old_word;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	old_word = tok->word;
	new_word = ft_calloc(1, sizeof(char));
	if (new_word == NULL)
		fatal_error("calloc");
	while (*old_word && !is_metacharacter(*old_word))
	{
		if (*old_word == SINGLE_QUOTE_CHAR)
			append_single_quote(&new_word, &old_word, old_word);
		else if (*old_word == DOUBLE_QUOTE_CHAR)
			append_double_quote(&new_word, &old_word, old_word);
		else if (is_variable(old_word))
			expand_variable_str(&new_word, &old_word, old_word);
		else if (is_macro(old_word))
			expand_macro(&new_word, &old_word, old_word);
		else
			append_char(&new_word, *old_word++);
	}
	free(tok->word);
	tok->word = new_word;
	expand_variable_tok(tok->next);
}

void	quote_removal(t_token *tok)
{
	char	*new_word;
	char	*p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = NULL;
	while (*p && !is_metacharacter(*p))
	{
		if (*p == SINGLE_QUOTE_CHAR)
		{
			// skip quote
			p++;
			while (*p != SINGLE_QUOTE_CHAR)
			{
				if (*p == '\0')
					assert_error("Unclosed single quote");
				append_char(&new_word, *p++);
			}
			// skip quote
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
			// skip quote
			p++;
		}
		else
			append_char(&new_word, *p++);
	}
	free(tok->word);
	tok->word = new_word;
	quote_removal(tok->next);
}

// nodeに含まれるtokenを展開する
void	expand_variable(t_node *node)
{
	if (node == NULL)
		return ;
	expand_variable_tok(node->args);
	expand_variable_tok(node->filename);
	// do not expand heredoc delimiter	expand_variable(node->redirect);
	expand_variable(node->command);
	expand_variable(node->next);
}

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

t_node	*expand(t_node *node)
{
	expand_variable(node);
	remove_quote(node);
	return (NULL);
}
