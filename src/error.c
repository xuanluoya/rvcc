#include "rvcc.h"

void
error (char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  vfprintf (stderr, fmt, ap);
  fprintf (stderr, "\n");
  va_end (ap);
  exit (1);
}

void
verror_at (char *loc, char *fmt, va_list ap)
{
  // The source message.
  fprintf (stderr, "%s\n", CurrentInput);

  // Output error message.
  // Calculate position
  int pos = loc - CurrentInput;
  fprintf (stderr, "%*s", pos, "");
  fprintf (stderr, "^ ");
  vfprintf (stderr, fmt, ap);
  fprintf (stderr, "\n");
  va_end (ap);
}

void
error_at (char *loc, char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  verror_at (loc, fmt, ap);
  exit (1);
}

void
error_tok (Token *tok, char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  verror_at (tok->loc, fmt, ap);
  exit (1);
}
