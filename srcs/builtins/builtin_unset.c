/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/13 22:36:32 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 

extern t_map *envmap;

int builtin_unset(char **argv)
{
	int i;
	int status;

	status = SUCCESS;
	i = 0;
	while (argv[i])
	{
		if (remove_var(envmap, argv[i]) == FAILURE)
		{
			dprintf(STDERR_FILENO, "unset: '%s': not a valid identifier\n", argv[i]);
			status = FAILURE;
		}
		else
			status = FAILURE;
		i++;
	}
	return (status);
}
