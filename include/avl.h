/*avl.h*/
#ifndef AVL_H
#define AVL_H 1

/* Function types. */
typedef int avl_comparison_func (const void *avl_a, const void *avl_b,void *avl_param);

/* Maximum AVL height. */
#ifndef AVL_MAX_HEIGHT
#define AVL_MAX_HEIGHT 32
#endif

/* Tree data structure. */
struct avl_table
{
    struct avl_node *avl_root;          /* Tree's root. */
    avl_comparison_func *avl_compare;   /* Comparison function. */
    void *avl_param;                    /* Extra argument to |avl_compare|. */
    size_t avl_count;                   /* Number of items in tree. */
    unsigned long avl_generation;       /* Generation number. */
};

/* An AVL tree node. */
struct avl_node
{
    struct avl_node *avl_link[2];  /* Subtrees. */
	void *avl_data;                /* Pointer to data. */
    signed char avl_balance;       /* Balance factor. */
};

/* Table functions. */
void **avl_probe (struct avl_table *, struct avl_node*);
void *avl_insert (struct avl_table *, struct avl_node*);
void *avl_replace (struct avl_table *, struct avl_node*);
struct avl_node*avl_delete (struct avl_table *, const void*);
struct avl_node *avl_find (const struct avl_table *,const void*);

#endif /* avl.h */
