#include "rvcc.h"

/**
 * @brief Initialize new token.
 *
 * @param kind The token kind.
 * @param start position of pointer.
 * @param end the end position of pointer.
 * @return Token* Pointer to the newly created token.
 */
static Token *
new_token (TokenKind kind, char *start, char *end)
{
  Token *tok = calloc (1, sizeof (Token));
  tok->kind = kind;
  tok->loc = start;
  tok->len = end - start;
  return tok;
}

/**
 * @brief Get token number if kinds is TK_NUM.
 *
 * @param tok token
 * @return int token number
 */
// static int
// get_number (Token *tok)
// {
//   if (tok->kind != TK_NUM)
//     error_tok (tok, "expect a number");
//   return tok->val;
// }

/**
 * @brief Determines whether the value is a given value.
 *
 * @param tok token
 * @param str string to compare
 * @return bool true if equal, false otherwise
 */
bool
equal (Token *tok, char *str)
{
  return memcmp (tok->loc, str, tok->len) == 0 && str[tok->len] == '\0';
};

/**
 * @brief If whether the value is given value then skip.
 *
 * @param tok token
 * @param str string to compare
 * @return Token* Pointer to the next token.
 */
Token *
skip (Token *tok, char *str)
{
  if (!equal (tok, str))
    error_tok (tok, "expect '%s'", str);
  return tok->next;
}

/**
 * @brief Determine whether it start with str/sub_str
 *
 * @param str
 * @param sub_str
 */
static bool
starts_with (char *str, char *sub_str)
{
  return strncmp (str, sub_str, strlen (sub_str)) == 0;
}

/**
 * @brief Read punct symbol.
 *
 * @param ptr String input.
 */
static int
read_punct (char *ptr)
{
  // 2 byte punct.
  if (starts_with (ptr, "==") || starts_with (ptr, ">=")
      || starts_with (ptr, "<=") || starts_with (ptr, "!="))
    {
      return 2;
    }

  // 1 byte.
  return ispunct (*ptr) ? 1 : 0;
}

Token *
tokenize (char *p)
{
  // (HEAD) -> 1 -> 2 -> 3
  Token head = {};
  Token *cur = &head;

  while (*p)
    {
      // skip whitespace.
      if (isspace (*p))
        {
          ++p;
          continue;
        }

      // Analysis number.
      if (isdigit (*p))
        {
          cur->next = new_token (TK_NUM, p, p);
          cur = cur->next;
          const char *oldptr = p;
          cur->val = strtol (p, &p, 10);
          cur->len = p - oldptr;
          continue;
        }

      // Analysis identifier.
      int punct_len = read_punct (p);
      if (punct_len)
        {
          cur->next = new_token (TK_PUNCT, p, p + punct_len);
          cur = cur->next;

          p += punct_len;
          continue;
        }

      // Handing unrecognizable character.
      error_at (p, "invalid token");
    }

  // Analysis end.
  // Add EOF
  cur->next = new_token (TK_EOF, p, p);

  // return head -> cur 1 (next) -> 2  ...
  return head.next;
}
