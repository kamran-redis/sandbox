#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intervaltree.h"

static bool nodeprint( const node  *x, const node *parent, char *side, int height, void *f_data) {
    //unused warning
    ((void) f_data);
    ((void) parent);

    char padding[height +1];
    memset(padding, ' ',height * sizeof(char));
    padding[height] = '\0';
    printf("%s%s %lld %lld %lld %lld\n", padding, side, x->lo, x->hi, x->max, x->value);
    return true;
}

void testdelete(void)
{
  printf("Staring...\n");
  IntervalTree *it = intervalTreeCreate();
  printf("insert 1\n");
  intervalTreePut(it, 5, 10, 0);
 
  printf("insert 2\n");
  intervalTreePut(it, 15, 20, 0);
  intervaltreeInOrder(it,nodeprint, " ");
  
  printf("search 1\n");//true
  printf ("%d \n", intervalTreeContains(it, 7,9));

  printf("search 2\n");//true
  printf ("%d \n", intervalTreeContains(it, 17, 17));


  printf("search 3\n");//true
  printf ("%d \n", intervalTreeContains(it, 1, 17));

  printf("search 4\n");//false
  printf ("%d \n", intervalTreeContains(it, 1, 3));



  printf(" delete 1\n");
  intervaltreeDelete(it, 5, 10);
  intervaltreeInOrder(it,nodeprint, "");
  printf(" delete 2\n");
  intervaltreeDelete(it, 15, 20);
  printf(" print 2\n");
  intervaltreeInOrder(it,nodeprint, "");
  printf(" delete 3 \n");
  intervaltreeDelete(it, 15, 20);

  printf("search 1\n");//false
  printf ("%d \n", intervalTreeContains(it, 7,9));

  printf("search 2\n");//false
  printf ("%d \n", intervalTreeContains(it, 17, 17));


  printf("search 3\n");//false
  printf ("%d \n", intervalTreeContains(it, 1, 17));

  printf("search 4\n");//false
  printf ("%d \n", intervalTreeContains(it, 1, 3));
  intervalTreeRelease(it);
}

void test(void)
{
  int N = 100000;
  srand((unsigned)time(NULL));
  IntervalTree *it = intervalTreeCreate();

  for (int i = 0; i < N; i++)
  {
    Item lo = rand() % N;
    Item hi = lo + 50;
    Value v = hi + 5;
    intervalTreePut(it, lo, hi, v);
  }
 intervaltreeInOrder(it,nodeprint, "");
  printf("Count: %d \n", intervaltreeCount(it));
  printf("Height: %d \n", intervaltreeHeight(it));
  printf("Check is %d \n", intervaltreeCheck(it));
  
  printf("Deleting tree \n");
  intervalTreeRelease(it);
  //intervalTreeSearch(it, 20);
  // intervalTreeSearch(it, 9);
  // intervalTreeSearch(it, 25);
  // intervalTreeDelete(it, 5);

}



int main(void)
{
  testdelete();
  test();
  printf("%lu \n", sizeof(struct node));
  //printf("%lu \n", sizeof(IntervalTree));

  IntervalTree *it = intervalTreeCreate();
  printf("%lu \n", sizeof(it));
  printf("%lu \n", sizeof(node));
  intervalTreeRelease(it);
  return 0;
}
