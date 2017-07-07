/* Public header */
#include "rbattle.h"


/* Dynamic matrix allocation */
result_t ** mkmatrix (unsigned rows, unsigned cols)
{
  unsigned r;

  /* The table of rows pointers */
  result_t ** matrix = calloc (rows, sizeof (result_t *));

  /* Rows allocation */
  for (r = 0; r < rows; r ++)
    matrix [r] = calloc (cols, sizeof (result_t));

  return matrix;
}


/* Dynamic matrix free */
void rmmatrix (result_t ** matrix, unsigned rows)
{
  if (matrix)
    {
      unsigned r;
      for (r = 0; r < rows; r ++)
	free (matrix [r]);
      free (matrix);
    }
}


/* Sort test results by avg time spent */
int sort_by_avg (const void * a, const void * b)
{
  if (! a || ! ((result_t *) a) -> test || ! ((result_t *) a) -> test -> avg ||
      ! b || ! ((result_t *) b) -> test || ! ((result_t *) b) -> test -> avg)
    return 0;

  return ((result_t *) a) -> test -> avg - ((result_t *) b) -> test -> avg;
}


int sort_by_rev_avg (const void * a, const void * b)
{
  return sort_by_avg (b, a);
}


int sort_by_items (const void * a, const void * b)
{
  if (! a || ! ((result_t *) a) -> test ||
      ! b || ! ((result_t *) b) -> test)
    return 0;

  return ((result_t *) a) -> test -> items - ((result_t *) b) -> test -> items;
}


int sort_by_rev_items (const void * a, const void * b)
{
  return sort_by_items (b, a);
}


int sort_by_spent (const void * a, const void * b)
{
  if (! a || ! ((result_t *) a) -> test ||
      ! b || ! ((result_t *) b) -> test)
    return 0;

  return ((result_t *) a) -> test -> elapsed - ((result_t *) b) -> test -> elapsed;
}


int sort_by_less_spent (const void * a, const void * b)
{
  return sort_by_spent (b, a);
}
