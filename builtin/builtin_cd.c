/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 19:22:23 by kishizu           #+#    #+#             */
/*   Updated: 2024/02/18 18:13:58 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"


void	builtin_cd(char *arg)
{
	if (chdir(arg) != 0)
	{
		perror("chdir");
		exit(EXIT_FAILURE);
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void test_builtin_cd() {
    char *initialDirectory = "/Users/kishizu/Desktop/testuser/42cursus/minishell"; // 初期ディレクトリのパスを指定
    char *targetDirectory = "/";   // 移動先ディレクトリのパスを指定

    printf("Testing cd command...\n");

    // テスト前の初期ディレクトリを表示
    printf("Initial directory: %s\n", initialDirectory);

    // builtin_cd関数を呼び出してディレクトリを変更
    builtin_cd(targetDirectory);

    // ディレクトリ変更後のカレントディレクトリを表示
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current directory after cd command: %s\n", cwd);
    } else {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    printf("cd command test completed.\n");
}

int main() {
    test_builtin_cd();
    return 0;
}
