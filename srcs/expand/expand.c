/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/03/18 01:26:17 by yutakagi         ###   ########.fr       */
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

void expand_macro(char **dst, char **rest, char *p)
{
	extern t_status	g_status;
	char			*temp;

	if (is_macro(p) == false)
		assert_error("Expected $? macro");
	p += ft_strlen("$?");
	temp = *dst;
	*dst = ft_itoa(g_status.exit_status);
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

t_node	*expand(t_node *node)
{
	expand_variable(node);
	// show_node(node);
	remove_quote(node);
	return (NULL);
}
