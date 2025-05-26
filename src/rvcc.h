/**
 * @file rvcc.h
 * @brief Include base head file and public function.
 */

#ifndef RVCC_H
#define RVCC_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Token struct.
 *
 * This enumeration is used to define token types.
 * @enum TokenKind
 */
typedef enum
{
  TK_PUNCT, /**< Operator: `+`, `-`... */
  TK_NUM,   /**< Number */
  TK_EOF,   /**< End of file */
} TokenKind;

/**
 * @brief Token struct.
 *
 * This struct defines a token data structure.
 *
 * @struct Token
 */
typedef struct Token
{
  TokenKind kind;     /**< Token kind. */
  struct Token *next; /**< The next node. */
  int val;            /**< Value. */
  char *loc;          /**< The position in string. */
  int len;            /**< Length of token. */
} Token;

// @brief Input's string.
extern char *CurrentInput;

/**
 * @brief Print an error message and exit.
 *
 * @param[in] fmt Format string for the error message.
 * @param[in] ... Additional arguments for the format string.
 */
void error (char *fmt, ...);

/**
 * @brief Output the location where the error occurred.
 *
 * @param[in] input Sourse input.
 * @param[in] loc Location for the error message.
 * @param[in] fmt Format string for the error message.
 * @param[in] ap Variable argument list for the format string.
 */
void verror_at (char *loc, char *fmt, va_list ap);

/**
 * @brief Output string analysos error, and quit.
 *
 * @param[in] loc Location for the error message.
 * @param[in] fmt Format string for the error message.
 */
void error_at (char *loc, char *fmt, ...);

/**
 * @brief Output token analysos error, and quit.
 *
 * @param[in] loc Location for the error message.
 * @param[in] fmt Format string for the error message.
 */
void error_tok (Token *tok, char *fmt, ...);

/**
 * @brief Initialize new token.
 *
 * @param[in] kind The token kind.
 * @param[in] start position of pointer.
 * @param[in] end the end position of pointer.
 * @return Token* Pointer to the newly created token.
 */
Token *new_token (TokenKind kind, char *start, char *end);

/**
 * @brief Get token number if kinds is TK_NUM.
 *
 * @param[in] tok token
 * @return int token number
 */
int get_number (Token *tok);

/**
 * @brief Determines whether the value is a given value.
 *
 * @param[in] tok token
 * @param[in] str string to compare
 * @return bool true if equal, false otherwise
 */
bool equal (Token *tok, char *str);

/**
 * @brief If whether the value is given value then skip.
 *
 * @param[in] tok token
 * @param[in] str string to compare
 * @return Token* Pointer to the next token.
 */
Token *skip (Token *tok, char *str);

/**
 * @brief Analysis string to the token.
 *
 * This function is used to analyze tokens.
 *
 * @param[in] p Is pointer to the string.
 * @return Token linked list.
 */
Token *tokenize (char *p);

/**
 * @brief Debug token.
 *
 * Show token information.
 *
 * @param[in] tok Pointer to the token.
 */
// void debug_token (Token *tok);

#endif
