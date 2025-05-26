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

/**
 * @brief Print an error message and exit.
 *
 * @param[in] fmt Format string for the error message.
 * @param[in] ... Additional arguments for the format string.
 */
static void error (char *fmt, ...);

/**
 * @brief Initialize new token.
 *
 * @param[in] kind The token kind.
 * @param[in] start position of pointer.
 * @param[in] end the end position of pointer.
 * @return Token* Pointer to the newly created token.
 */
static Token *new_token (TokenKind kind, char *start, char *end);

/**
 * @brief Get token number if kinds is TK_NUM.
 *
 * @param[in] tok token
 * @return int token number
 */
static int get_number (Token *tok);

/**
 * @brief Determines whether the value is a given value.
 *
 * @param[in] tok token
 * @param[in] str string to compare
 * @return bool true if equal, false otherwise
 */
static bool equal (Token *tok, char *str);

/**
 * @brief If whether the value is given value then skip.
 *
 * @param[in] tok token
 * @param[in] str string to compare
 * @return Token* Pointer to the next token.
 */
static Token *skip (Token *tok, char *str);

/**
 * @brief Analysis string to the token.
 *
 * This function is used to analyze tokens.
 *
 * @param[in] p Is pointer to the string.
 * @return Token linked list.
 */
static Token *tokenize (char *p);

/**
 * @brief Debug token.
 *
 * Show token information.
 *
 * @param[in] tok Pointer to the token.
 */
// static void debug_token (Token *tok);

#endif
