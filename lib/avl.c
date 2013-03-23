#include <stddef.h>
#include <avl.h>

/* Search |tree| for an item matching |item|, and return it if found.
   Otherwise return |NULL|. */
struct avl_node *avl_find (const struct avl_table *tree, const void *item)
{
  struct avl_node *p;

//  assert (tree != NULL && item != NULL);
  for (p = tree->avl_root; p != NULL; )
    {
      int cmp = tree->avl_compare (item, p->avl_data, tree->avl_param);
      if (cmp < 0)
        p = p->avl_link[0];
      else if (cmp > 0)
        p = p->avl_link[1];
      else /* |cmp == 0| */
        return p;
    }
  return NULL;
}

/* Inserts |item| into |tree| and returns a pointer to |item|'s address.
   If a duplicate item is found in the tree,
   returns a pointer to the duplicate without inserting |item|.
   Returns |NULL| in case of memory allocation failure. */
void **avl_probe (struct avl_table *tree, struct avl_node* item)
{
  struct avl_node *y, *z; /* Top node to update balance factor, and parent. */
  struct avl_node *p, *q; /* Iterator, and parent. */
  struct avl_node *n;     /* Newly inserted node. */
  struct avl_node *w;     /* New root of rebalanced subtree. */
  int dir;                /* Direction to descend. */

  unsigned char da[AVL_MAX_HEIGHT]; /* Cached comparison results. */
  int k = 0;              /* Number of cached results. */

//  assert (tree != NULL && item != NULL);

  z = (struct avl_node *) &tree->avl_root;
  y = tree->avl_root;
  dir = 0;
  for (q = z, p = y; p != NULL; q = p, p = p->avl_link[dir])
    {
      int cmp = tree->avl_compare (item->avl_data, p->avl_data, tree->avl_param);
      if (cmp == 0)
        return &p->avl_data;

      if (p->avl_balance != 0)
        z = q, y = p, k = 0;
      da[k++] = dir = cmp > 0;
    }

  n = q->avl_link[dir] = item;

  if (n == NULL)
    return NULL;

  tree->avl_count++;
  n->avl_data = item->avl_data;
  n->avl_link[0] = n->avl_link[1] = NULL;
  n->avl_balance = 0;
  if (y == NULL)
    return &n->avl_data;

  for (p = y, k = 0; p != n; p = p->avl_link[da[k]], k++)
    if (da[k] == 0)
      p->avl_balance--;
    else
      p->avl_balance++;

  if (y->avl_balance == -2)
    {
      struct avl_node *x = y->avl_link[0];
      if (x->avl_balance == -1)
        {
          w = x;
          y->avl_link[0] = x->avl_link[1];
          x->avl_link[1] = y;
          x->avl_balance = y->avl_balance = 0;
        }
      else
        {
 //         assert (x->avl_balance == +1);
          w = x->avl_link[1];
          x->avl_link[1] = w->avl_link[0];
          w->avl_link[0] = x;
          y->avl_link[0] = w->avl_link[1];
          w->avl_link[1] = y;
          if (w->avl_balance == -1)
            x->avl_balance = 0, y->avl_balance = +1;
          else if (w->avl_balance == 0)
            x->avl_balance = y->avl_balance = 0;
          else /* |w->avl_balance == +1| */
            x->avl_balance = -1, y->avl_balance = 0;
          w->avl_balance = 0;
        }
    }
  else if (y->avl_balance == +2)
    {
      struct avl_node *x = y->avl_link[1];
      if (x->avl_balance == +1)
        {
          w = x;
          y->avl_link[1] = x->avl_link[0];
          x->avl_link[0] = y;
          x->avl_balance = y->avl_balance = 0;
        }
      else
        {
 //         assert (x->avl_balance == -1);
          w = x->avl_link[0];
          x->avl_link[0] = w->avl_link[1];
          w->avl_link[1] = x;
          y->avl_link[1] = w->avl_link[0];
          w->avl_link[0] = y;
          if (w->avl_balance == +1)
            x->avl_balance = 0, y->avl_balance = -1;
          else if (w->avl_balance == 0)
            x->avl_balance = y->avl_balance = 0;
          else /* |w->avl_balance == -1| */
            x->avl_balance = +1, y->avl_balance = 0;
          w->avl_balance = 0;
        }
    }
  else
    return &n->avl_data;
  z->avl_link[y != z->avl_link[0]] = w;

  tree->avl_generation++;
  return &n->avl_data;
}

/* Inserts |item| into |table|.
   Returns |NULL| if |item| was successfully inserted
   or if a memory allocation error occurred.
   Otherwise, returns the duplicate item. */
void * avl_insert (struct avl_table *table, struct avl_node* item)
{
  void **p = avl_probe (table, item);
  return p == NULL || *p == item ? NULL : *p;
}

/* Inserts |item| into |table|, replacing any duplicate item.
   Returns |NULL| if |item| was inserted without replacing a duplicate,
   or if a memory allocation error occurred.
   Otherwise, returns the item that was replaced. */
void * avl_replace (struct avl_table *table, struct avl_node *item)
{
  void **p = avl_probe (table, item);
  if (p == NULL || *p == item)
    return NULL;
  else
    {
      void *r = *p;
      *p = item;
      return r;
    }
}

/* Deletes from |tree| and returns an item matching |item|.
   Returns a null pointer if no matching item found. */
struct avl_node *avl_delete (struct avl_table *tree, const void * item)
{
  /* Stack of nodes. */
  struct avl_node *pa[AVL_MAX_HEIGHT]; /* Nodes. */
  unsigned char da[AVL_MAX_HEIGHT];    /* |avl_link[]| indexes. */
  int k;                               /* Stack pointer. */

  struct avl_node *p;   /* Traverses tree to find node to delete. */
  int cmp;              /* Result of comparison between |item| and |p|. */

 // assert (tree != NULL && item != NULL);

  k = 0;
  p = (struct avl_node *) &tree->avl_root;
  for (cmp = -1; cmp != 0;
       cmp = tree->avl_compare (item, p->avl_data, tree->avl_param))
    {
      int dir = cmp > 0;

      pa[k] = p;
      da[k++] = dir;

      p = p->avl_link[dir];
      if (p == NULL)
        return NULL;
    }

  if (p->avl_link[1] == NULL)
    pa[k - 1]->avl_link[da[k - 1]] = p->avl_link[0];
  else
    {
      struct avl_node *r = p->avl_link[1];
      if (r->avl_link[0] == NULL)
        {
          r->avl_link[0] = p->avl_link[0];
          r->avl_balance = p->avl_balance;
          pa[k - 1]->avl_link[da[k - 1]] = r;
          da[k] = 1;
          pa[k++] = r;
        }
      else
        {
          struct avl_node *s;
          int j = k++;

          for (;;)
            {
              da[k] = 0;
              pa[k++] = r;
              s = r->avl_link[0];
              if (s->avl_link[0] == NULL)
                break;

              r = s;
            }

          s->avl_link[0] = p->avl_link[0];
          r->avl_link[0] = s->avl_link[1];
          s->avl_link[1] = p->avl_link[1];
          s->avl_balance = p->avl_balance;

          pa[j - 1]->avl_link[da[j - 1]] = s;
          da[j] = 1;
          pa[j] = s;
        }
    }

	item = p;

 // assert (k > 0);
  while (--k > 0)
    {
      struct avl_node *y = pa[k];

      if (da[k] == 0)
        {
          y->avl_balance++;
          if (y->avl_balance == +1)
            break;
          else if (y->avl_balance == +2)
            {
              struct avl_node *x = y->avl_link[1];
              if (x->avl_balance == -1)
                {
                  struct avl_node *w;
                 // assert (x->avl_balance == -1);
                  w = x->avl_link[0];
                  x->avl_link[0] = w->avl_link[1];
                  w->avl_link[1] = x;
                  y->avl_link[1] = w->avl_link[0];
                  w->avl_link[0] = y;
                  if (w->avl_balance == +1)
                    x->avl_balance = 0, y->avl_balance = -1;
                  else if (w->avl_balance == 0)
                    x->avl_balance = y->avl_balance = 0;
                  else /* |w->avl_balance == -1| */
                    x->avl_balance = +1, y->avl_balance = 0;
                  w->avl_balance = 0;
                  pa[k - 1]->avl_link[da[k - 1]] = w;
                }
              else
                {
                  y->avl_link[1] = x->avl_link[0];
                  x->avl_link[0] = y;
                  pa[k - 1]->avl_link[da[k - 1]] = x;
                  if (x->avl_balance == 0)
                    {
                      x->avl_balance = -1;
                      y->avl_balance = +1;
                      break;
                    }
                  else
                    x->avl_balance = y->avl_balance = 0;
                }
            }
        }
      else
        {
          y->avl_balance--;
          if (y->avl_balance == -1)
            break;
          else if (y->avl_balance == -2)
            {
              struct avl_node *x = y->avl_link[0];
              if (x->avl_balance == +1)
                {
                  struct avl_node *w;
                 // assert (x->avl_balance == +1);
                  w = x->avl_link[1];
                  x->avl_link[1] = w->avl_link[0];
                  w->avl_link[0] = x;
                  y->avl_link[0] = w->avl_link[1];
                  w->avl_link[1] = y;
                  if (w->avl_balance == -1)
                    x->avl_balance = 0, y->avl_balance = +1;
                  else if (w->avl_balance == 0)
                    x->avl_balance = y->avl_balance = 0;
                  else /* |w->avl_balance == +1| */
                    x->avl_balance = -1, y->avl_balance = 0;
                  w->avl_balance = 0;
                  pa[k - 1]->avl_link[da[k - 1]] = w;
                }
              else
                {
                  y->avl_link[0] = x->avl_link[1];
                  x->avl_link[1] = y;
                  pa[k - 1]->avl_link[da[k - 1]] = x;
                  if (x->avl_balance == 0)
                    {
                      x->avl_balance = +1;
                      y->avl_balance = -1;
                      break;
                    }
                  else
                    x->avl_balance = y->avl_balance = 0;
                }
            }
        }
    }

  tree->avl_count--;
  tree->avl_generation++;
  return (void *) item;
}
