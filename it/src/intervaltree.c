/*
 * Implementation of an interval tree based on left leaning Red-Black tree.
 * TODO: check free specially with delete
 */


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intervaltree.h"

#ifndef INTERVALTREE_MALLOC_INCLUDE
#define INTERVALTREE_MALLOC_INCLUDE "intervaltree_redismalloc.h"
#endif
#include INTERVALTREE_MALLOC_INCLUDE


#define RED 1
#define BLACK 0
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CMP(a, b) ((((a) < (b))) ? (-1) : ((a) > (b)))


/******************************************************************************
 * Small helper functions
 ******************************************************************************/

static bool isRed(const node *n) {
  // null links are black
  if (n == NULL)
    return BLACK;
  return n->color;
}

static Item maxValue(const node *n) {
  if (n == NULL)
    return 0;
  return n->max;
}

static int max2(Item a, Item b) { return (a > b) ? a : b; }

// TODO:change types for internal use
static Item max3(Item a, Item b, Item c) { return MAX(a, MAX(b, c)); }

static int compareNode(node *a, node *b) { return CMP(a->lo, b->lo); }

static int count(node *x) {
  if (x == NULL)
    return 0;
  return x->count;
}

static bool intersects(Item lo1, Item hi1, Item lo2, Item hi2) {
  if (hi1 < lo2)
    return false;
  if (hi2 < lo1)
    return false;

  return true;
}

/******************************************************************************
 *
 *  RB Tree internal balance operations
 *
 ******************************************************************************/

/*
Orient a temporarily right leaning red link to lean left
mantains symmetric order
9:29
*/
static node *rotateLeft(node *h) {
  node *x = h->right;
  h->right = x->left;
  x->left = h;
  x->color = h->color;
  h->color = RED;

  // h is child of x now , update h first
  h->count = 1 + count(h->left) + count(h->right);
  h->max = max3(h->hi, maxValue(h->left), maxValue(h->right));
  x->count = 1 + count(x->left) + count(x->right);
  x->max = max3(x->hi, maxValue(x->left), maxValue(x->right));
  return x;
}

/*
 * Temporarily rotate left leaning red link to right
 *10.08
 */
static node *rotateRight(node *h) {
  node *x = h->left;
  h->left = x->right;
  x->right = h;
  x->color = h->color;
  h->color = RED;

  h->count = 1 + count(h->left) + count(h->right);
  h->max = max3(h->hi, maxValue(h->left), maxValue(h->right));
  x->count = 1 + count(x->left) + count(x->right);
  x->max = max3(x->hi, maxValue(x->left), maxValue(x->right));

  return x;
}

/*
 *
 */
static void flipColors(node *h) {
  h->color = !h->color;               // black to red
  h->left->color = !h->left->color;   // red to black
  h->right->color = !h->right->color; // red to black
}

static node *moveRedLeft(node *h) {
  flipColors(h);
  if (isRed(h->right->left)) {
    h->right = rotateRight(h->right);
    h = rotateLeft(h);
  }
  return h;
}

static node *moveRedRight(node *h) {
  flipColors(h);
  if (isRed(h->left->left)) {
    h = rotateRight(h);
  }
  return h;
}

static node *balance(node *h) {
  if (isRed(h->right))
    h = rotateLeft(h);
  if (isRed(h->left) && isRed(h->left->left))
    h = rotateRight(h);
  if (isRed(h->left) && isRed(h->right))
    flipColors(h);
  h->count = count(h->left) + count(h->right) + 1;
  h->max = max3(h->hi, maxValue(h->left), maxValue(h->right));
  return h;
}

/******************************************************************************
 * DM Operations
 ******************************************************************************/
IntervalTree *intervalTreeCreate() {
  IntervalTree *it = intervaltree_malloc(sizeof(struct interval_type));
  if (it == NULL) {
    //ToDo: better error handling
    printf("could not create IntervalTree");
  }

  it->root = NULL;
  return it;
}

bool intervalTreeIsEmpty(const IntervalTree *it) { return it->root == NULL; }

int intervaltreeCount(const IntervalTree *it) { return count(it->root); }

static bool inorder(node *x, node *parent, char *side, int level,
                    bool (*f)(const node *, const node *, char *, int, void *), void *f_data) {
  if (x == NULL)
    return true;

  
  if (!inorder(x->left, x,"L", level + 1, f, f_data))
    return false;
  if (!f(x, parent, side, level, f_data))
    return false;
  if (!inorder(x->right, x, "R", level + 1, f, f_data))
    return false;
  return true;
}

void intervaltreeInOrder(const IntervalTree *it,
                         bool (*f)(const node *, const node *, char *, int, void *),
                         void *f_data) {
  inorder(it->root,  NULL, "", 0, f, f_data);
}

static node *put(node *x, Item lo, Item hi, Value val) {
  if (x == NULL) {
    // TODO check of OOM
    x = intervaltree_malloc(sizeof(struct node));
    x->lo = lo;
    x->hi = x->max = hi;
    x->value = val;
    x->count = 1;
    x->right = x->left = NULL;
    x->color = RED;
    return x;
  }

  int cmp = CMP(lo, x->lo);
  if (cmp < 0)
    x->left = put(x->left, lo, hi, val);
  else if (cmp > 0)
    x->right = put(x->right, lo, hi, val);
  else
    x->value = val;

  x->count = 1 + count(x->left) + count(x->right);
  x->max = max3(x->hi, maxValue(x->left), maxValue(x->right));

  if (isRed(x->right) && !isRed(x->left))
    x = rotateLeft(x);
  if (isRed(x->left) && isRed(x->left->left))
    x = rotateRight(x);
  if (isRed(x->left) && isRed(x->right))
    flipColors(x);

  return x;
}

int intervalTreePut(IntervalTree *it, Item lo, Item hi, Value value) {
  it->root = put(it->root, lo, hi, value);
  it->root->color = BLACK;
  return 1; // TODO better return value
}

static node *minNode(node *x) {
  if (x->left == NULL)
    return x;
  else
    return minNode(x->left);
}

static node *deleteMinNode(node *h) {
  if (h->left == NULL)
    intervaltree_free(h);
  return NULL;
  if (!isRed(h->left) && !isRed(h->left->left))
    h = moveRedLeft(h);
  h->left = deleteMinNode(h->left);
  return balance(h);
}
static node *delete (node *h, Item lo, Item hi) {
  if (CMP(lo, h->lo) < 0) {
    if (!isRed(h->left) && !isRed(h->left->left)) {
      h = moveRedLeft(h);
    }
    h->left = delete (h->left, lo, hi);
  } else {
    if (isRed(h->left)) {
      h = rotateRight(h);
    }
    if ((CMP(lo, h->lo) == 0) && (h->right == NULL)) {
      intervaltree_free(h);
      return NULL;
    }
    if (!isRed(h->right) && !isRed(h->right->left)) {
      h = moveRedRight(h);
    }

    if ((CMP(lo, h->lo)) == 0) {
      node *x = minNode(h->right);
      h->lo = x->lo;
      h->hi = x->hi;
      h->value = x->value;
      // TODO: other values
      h->right = deleteMinNode(h->right);
    } else {
      h->right = delete (h->right, lo, hi);
    }
  }
  return balance(h);
}

void intervaltreeDelete(IntervalTree *it, Item lo, Item hi) {
  if (intervalTreeIsEmpty(it))
    return;

  if (!isRed(it->root->left) && !isRed(it->root->right))
    it->root->color = RED;
  it->root = delete (it->root, lo, hi);
  if (!intervalTreeIsEmpty(it))
    it->root->color = BLACK;
}

static bool searchNode(node *x, Item lo, Item hi) {
  while (x != NULL) {
    if (intersects(lo, hi, x->lo, x->hi))
      return true;
    else if (x->left == NULL)
      x = x->right;
    else if (x->left->max < lo)
      x = x->right;
    else
      x = x->left;
  }
  return false;
}

bool intervalTreeContains(const IntervalTree *it, Item lo, Item hi) {
  return searchNode(it->root, lo, hi);
}


static bool search(node *x, Item lo, Item hi, const int limit, int *n, bool (*f)(const node *, void *), void *f_data) {
  if (*n == limit) return false;
  bool found1 = false;
  bool found2 = false;
  bool found3 =false;
  if (x == NULL) return false;
  
  if  (intersects(lo, hi, x->lo, x->hi)) {
    (*n)++;
    f(x, f_data);
    found1 = true;
  }

  if (x->left != NULL && x->left->max >= lo)
    found2 = search(x->left, lo, hi, limit,n, f, f_data);
  if (found2 || x->left == NULL ||x->left->max < hi)
    found3 = search(x->right, lo, hi, limit, n, f, f_data);
  return found1 || found2||found3;
}

int IntervalTreeSearch(const IntervalTree *it, Item lo, Item hi, int limit, bool (*f)(const node *, void *), void *f_data) {
  int n = 0;
  search(it->root, lo, hi, limit, &n, f, f_data);
  return n;
}

static void deleteNode(node *x) {
  if (x == NULL)
    return;

  deleteNode(x->left);
  deleteNode(x->right);
  intervaltree_free(x);
}
void intervalTreeRelease(IntervalTree *it) {
  deleteNode(it->root);
  it->root = NULL;
  intervaltree_free(it);
}

/******************************************************************************
 * Check data structure are valid
 *******************************************************************************/

int nodeHeight(node *x) {
  if (x == NULL)
    return -1;
  return 1 + max2(nodeHeight(x->left), nodeHeight(x->right));
}

int intervaltreeHeight(const IntervalTree *it) { return nodeHeight(it->root); }

static bool isNodeBST(node *x, node *min, node *max) {
  if (x == NULL)
    return true;
  if (min != NULL && compareNode(x, min) <= 0)
    return false;
  if (max != NULL && compareNode(x, max) >= 0)
    return false;
  return isNodeBST(x->left, min, x) && isNodeBST(x->right, x, max);
}

bool intervaltreeIsBST(const IntervalTree *it) {
  return isNodeBST(it->root, NULL, NULL);
}

static bool isNodeSizeConsistent(node *x) {
  if (x == NULL)
    return true;
  if (x->count != count(x->left) + count(x->right) + 1)
    return false;
  return isNodeSizeConsistent(x->left) && isNodeSizeConsistent(x->right);
}

bool intervaltreeIsSizeConsistent(const IntervalTree *it) {
  return isNodeSizeConsistent(it->root);
}

static bool isNodeMaxConsistent(node *x) {
  if (x == NULL)
    return true;
  if (x->max != max3(x->hi, maxValue(x->left), maxValue(x->right)))
    return false;
  return isNodeMaxConsistent(x->left) && isNodeMaxConsistent(x->right);
}

bool intervaltreeIsMaxConistent(const IntervalTree *it) {
  return isNodeMaxConsistent(it->root);
}

static bool isNodeBalanced(node *x, int black) {
  if (x == NULL)
    return black == 0;
  if (!isRed(x))
    black--;
  return isNodeBalanced(x->left, black) && isNodeBalanced(x->right, black);
}

bool intervaltreeIsBalanced(const IntervalTree *it) {
  int black = 0; // number of black links on path from root to min
  node *x = it->root;
  while (x != NULL) {
    if (!isRed(x))
      black++;
    x = x->left;
  }
  return isNodeBalanced(it->root, black);
}

static bool isNode23(node *x, node *root) {
  if (x == NULL)
    return true;
  if (isRed(x->right))
    return false;
  if (x != root && isRed(x) && isRed(x->left))
    return false;

  return isNode23(x->left, root) && isNode23(x->right, root);
}
bool intervaltreeIs23(const IntervalTree *it) { return isNode23(it->root, it->root); }

bool intervaltreeCheck(const IntervalTree *it) {
  bool result = true;
  if (!intervaltreeIsBST(it)) {
    printf("Not a BST");
    result = false;
  }
  if (!intervaltreeIsMaxConistent(it)) {
    printf("Max not consitent");
    result = false;
  }
  if (!intervaltreeIsSizeConsistent(it)) {
    printf("Size not consistent");
    result = false;
  }

  if (!intervaltreeIsBalanced(it)) {
    printf(" Not balanced");
    result = false;
  }

  if (!intervaltreeIs23(it)) {
    printf(" Not 23");
    result = false;
  }

  return result;
}
