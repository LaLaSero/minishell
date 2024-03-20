/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 19:50:04 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/20 17:33:32 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
#include "../libft/libft.h"

void	expand_macro(char **dst, char **rest, char *p, int status)
{
	char			*temp;
	char			*tmp;

	p += ft_strlen("$?");
	temp = ft_itoa(status);
	tmp = temp;
	while (*temp)
		append_char(dst, *temp++);
	free(tmp);
	*rest = p;
}

void	expand_variable_str(char **dst, char **rest, int *error, t_map *envmap)
{
	char	*var_name;
	char	*value;
	char	*q;

	q = *rest;
	var_name = ft_calloc(1, sizeof(char));
	if (var_name == NULL)
		fatal_error("calloc");
	if (*q != '$')
		assert_error("Expected dollar sign", error);
	q++;
	if (!is_alpha_under(*q))
		assert_error("Variable must start with alphabet or underbar.", error);
	append_char(&var_name, *q++);
	while (is_alpha_num_under(*q))
		append_char(&var_name, *q++);
	value = get_value(var_name, envmap);
	free(var_name);
	if (value)
		while (*value)
			append_char(dst, *value++);
	else
		append_char(dst, '\0');
	*rest = q;
}

void	expand_variable_tok(t_token *tok, int status, int *error, t_map *envmap)
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
			append_single_quote(&new_word, &old_word, old_word, error);
		else if (*old_word == DOUBLE_QUOTE_CHAR)
			append_double_quote(&new_word, &old_word, error, envmap);
		else if (is_variable(old_word))
			expand_variable_str(&new_word, &old_word, error, envmap);
		else if (is_macro(old_word))
			expand_macro(&new_word, &old_word, old_word, status);
		else
			append_char(&new_word, *old_word++);
	}
	free(tok->word);
	tok->word = new_word;
	expand_variable_tok(tok->next, status, error, envmap);
}

void	expand_variable(t_node *node, int status, int *error, t_map *envmap)
{
	if (node == NULL)
		return ;
	expand_variable_tok(node->args, status, error, envmap);
	expand_variable_tok(node->filename, status, error, envmap);
	expand_variable(node->command, status, error, envmap);
	expand_variable(node->next, status, error, envmap);
}

t_node	*expand(t_node *node, int *status, int *error, t_map *envmap)
{
	envmap->exit_status = *status;
	expand_variable(node, *status, error, envmap);
	remove_quote(node, error);
	return (NULL);
}
