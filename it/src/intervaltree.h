#ifndef INTERVALTREE_H
#define INTERVALTREE_H
#include <stdbool.h>
#include <stdint.h>

typedef long long Item;
typedef long long Value;
typedef struct node node;
struct node {
  node *left, *right;  // left right nodes
  int count;           // count of nodes below this node
  int color;           // color of link
  Item max;            // max value
  Item hi;
  Item lo;
  Value value;
};

typedef struct interval_type IntervalTree;
struct interval_type {
  node *root;
};


IntervalTree *intervalTreeCreate(void);
int intervalTreePut(IntervalTree *it, Item lo, Item hi, Value value);
void intervaltreeDelete(IntervalTree *it, Item lo, Item hi);
bool intervalTreeContains(const IntervalTree *it, Item lo, Item hi);
bool intervalTreeIsEmpty(const IntervalTree *it);
void intervalTreeRelease(IntervalTree *it);
int intervaltreeHeight(const IntervalTree *it);
int intervaltreeCount(const IntervalTree *it);
bool intervaltreeCheck(const IntervalTree *it);
void intervaltreeInOrder(const IntervalTree *it,
                         bool (*f)(const node *x, const node *parent,
                                   char *side, int level, void *f_data),
                         void *f_data);
int IntervalTreeSearch(const IntervalTree *it, Item lo, Item hi, int limit,
                       bool (*f)(const node *, void *), void *f_data);

// Check debug functions
bool intervaltreeIsBST(const IntervalTree *it);
bool intervaltreeIs23(const IntervalTree *it);
bool intervaltreeIsBalanced(const IntervalTree *it);
bool intervaltreeIsMaxConistent(const IntervalTree *it);
bool intervaltreeIsSizeConsistent(const IntervalTree *it);
#endif