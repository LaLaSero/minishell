#include <string.h>
#include <stdlib.h>
#include "../includes/minishell.h"
#include <assert.h>
#include <stdbool.h>

// #define ERROR_PREFIX "minishell: "
void	perror_prefix(void)
{
	dprintf(STDERR_FILENO, "%s", ERROR_PREFIX);
}

void	fatal_error(const char *msg)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
	exit(1);
}

void	assert_error(const char *msg)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "Assert Error: %s\n", msg);
	exit(255);
}

t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*tok;

	tok = calloc(1, sizeof(*tok));
	if (tok == NULL)
		fatal_error("calloc");
	tok->word = word;
	tok->kind = kind;
	return (tok);
}

bool	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	consume_blank(char **rest, char *line)
{
	if (is_blank(*line))
	{
		while (*line && is_blank(*line))
			line++;
		*rest = line;
		return (true);
	}
	*rest = line;
	return (false);
}

bool	startswith(const char *s, const char *keyword)
{
	return (memcmp(s, keyword, strlen(keyword)) == 0);
}

/*
metacharacter
	  A character that, when unquoted, separates words.  One of the following:
	  |  & ; ( ) < > space tab
control operator
	  A token that performs a control function.  It is one of the following symbols:
	  || & && ; ;; ( ) | <newline>
*/
bool	is_operator(const char *s)
{
	static char	*const operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
	size_t				i = 0;				

	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(s, operators[i]))
			return (true);
		i++;
	}
	return (false);
}

/*
DEFINITIONS
       The following definitions are used throughout the rest of this document.
       blank  A space or tab.
       word   A sequence of characters considered as a single unit by the shell.  Also known as a token.
       name   A word consisting only of alphanumeric characters and underscores, and beginning with an alphabetic
              character or an underscore.  Also referred to as an identifier.
       metacharacter
              A character that, when unquoted, separates words.  One of the following:
              |  & ; ( ) < > space tab
       control operator
              A token that performs a control function.  It is one of the following symbols:
              || & && ; ;; ( ) | <newline>
*/
bool	is_metacharacter(char c)
{
	return (c && strchr("|&;()<> \t\n", c));
}

bool	is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}

t_token	*operator(char **rest, char *line)
{
	static char	*const	operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
	size_t				i = 0;				
	char				*op;

	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(line, operators[i]))
		{
			op = strdup(operators[i]);
			if (op == NULL)
				fatal_error("strdup");
			*rest = line + strlen(op);
			return (new_token(op, TK_OP));
		}
		i++;
	}
	assert_error("Unexpected operator");
	return (NULL);
}

#define SINGLE_QUOTE_CHAR '\''

t_token *word(char **rest, char *line) {
	const char *start = line;
	char *word;

	// printf("word_before: [%s]\n", line);
	while (*line && !is_metacharacter(*line) && *line != SINGLE_QUOTE_CHAR)
		line++;
	// printf("word_after: [%s]\n", line);
	if (*line == SINGLE_QUOTE_CHAR) {
		start++;
		line++;
		while (*line != SINGLE_QUOTE_CHAR) {
			if (*line == '\0') {
				fatal_error("Unclosed single quote");
			}
			line++;
		}
		line++;
	} else if (*line && !is_metacharacter(*line)) {
		line++;
	}
	size_t word_length = line - start;
	if (*(line - 1) == SINGLE_QUOTE_CHAR) {
		word_length--;
	}
	word = strndup(start, word_length);
	if (word == NULL) {
		fatal_error("strndup failed");
	}
	*rest = line;
	return new_token(word, TK_WORD);
}

// t_token	*word(char **rest, char *line)
// {
// 	const char	*start = line;
// 	char		*word;

// 	printf("word_before: [%s]\n", line);
// 	while (*line && !is_metacharacter(*line) && *line != SINGLE_QUOTE_CHAR)
// 		line++;
// 	printf("word_after: [%s]\n", line);
// 	if (*line == SINGLE_QUOTE_CHAR)
// 	{
// 		// skip quote
// 		start++;
// 		line++;
// 		while (*line != SINGLE_QUOTE_CHAR)
// 		{
// 			if (*line == '\0')
// 			{
// 				fatal_error("Unclosed single quote");
// 				exit(1);
// 			}
// 			line++;
// 		}
// 		// skip quote
// 		if (*line == SINGLE_QUOTE_CHAR)
// 			line++;
// 	}
// 	else if (*line != '\0' && !is_metacharacter(*line))
// 	// else if(*line == ' ')
// 		line++;
// 	if (*(line - 1) == SINGLE_QUOTE_CHAR)
// 		word = strndup(start, line - start -1);
// 	else
// 		word = strndup(start, line - start);
// 	if (word == NULL)
// 		fatal_error("strndup");
// 	*rest = line;
// 	return (new_token(word, TK_WORD));
// }

// t_token *word(char **rest, char *line) {
//     const char *start = line;
//     char *word = NULL;
//     bool in_quotes = false;
//     bool word_started = false;

//     while (*line) {
//         if (!in_quotes && is_metacharacter(*line)) {
//             // 単語の開始を検出していない、かつメタ文字に遭遇した場合、スキップする
//             if (!word_started) {
//                 line++;
//                 start = line; // スタートを更新
//                 continue;
//             }
//             break; // 単語の終了
//         } else if (*line == SINGLE_QUOTE_CHAR) {
//             if (!in_quotes && word_started) {
//                 // シングルクォートの前に単語が始まっている場合、その単語を終了させる
//                 break;
//             }
//             // シングルクォート内の処理を開始または終了
//             in_quotes = !in_quotes;
//             line++;
//             if (!in_quotes) {
//                 // 閉じクォート; 単語の終了
//                 break;
//             } else {
//                 // 開始クォート; 単語の開始
//                 start = line;
//                 word_started = true;
//                 continue;
//             }
//         } else {
//             if (!word_started) {
//                 start = line; // 単語の開始
//                 word_started = true;
//             }
//             line++;
//         }
//     }

//     if (word_started) {
//         // 単語を抽出してトークンを生成
//         size_t length = line - start - (in_quotes ? 1 : 0); // 閉じクォートを含めない
//         word = strndup(start, length);
//         *rest = line + (in_quotes ? 0 : 1); // 閉じクォートの後ろを指す
//         return new_token(word, TK_WORD);
//     }

//     *rest = line;
//     return NULL; // 単語が見つからない場合
// }


t_token	*tokenize(char *line)
{
	t_token	head;
	t_token	*tok;

	head.next = NULL;
	tok = &head;
	// printf("line: [%s]\n", line);
	while (*line)
	{
		// printf("line: [%s]\n", line);
		if (consume_blank(&line, line))
			continue ;
		else if (is_operator(line))
			tok = tok->next = operator(&line, line);
		else if (is_word(line))
			tok = tok->next = word(&line, line);
		else
			assert_error("Unexpected Token");
	}
	tok->next = new_token(NULL, TK_EOF);
	return (head.next);
}