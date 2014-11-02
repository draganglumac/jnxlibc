/*
 * =====================================================================================
 *
 *       Filename:  jnxquicksort.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/10/14 22:14:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (draganglumac), dragan.glumac@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "jnxquicksort.h"

typedef struct {
  void **array;
  compare_function cf;
} sortable_array;

static void kwiksort(sortable_array *sa, int32_t first, int32_t last); 
static int32_t choose_pivot(sortable_array *sa, int32_t first, int32_t last);
static void swap(sortable_array *sa, int32_t i, int32_t j);
static int32_t partition(sortable_array *sa, int32_t first, int32_t last);

void jnx_quicksort(void **array, int32_t size, compare_function cf) {
  if (array == 0 || size <= 0) {
    return;
  }
  sortable_array sa;
  sa.array = array;
  sa.cf = cf;
  kwiksort(&sa, 0, size - 1);
}
static void kwiksort(sortable_array *sa, int32_t first, int32_t last) {
  if (first >= last) {
    return;
  }
  int32_t pivot = partition(sa, first, last);
  kwiksort(sa, first, pivot - 1);
  kwiksort(sa, pivot + 1, last);
}
static int32_t calculate_midpoint32_t(int32_t first, int32_t last) {
  int32_t size = last - first + 1;
  if (size % 2 == 0) {
    return first + size/2 - 1;
  }
  else {
    return first + size/2;
  }
}
static int32_t choose_median_of_three(sortable_array *sa, int32_t first, int32_t last) {
  if (last - first > 1) {
    int32_t smallest = first, median = first;
    int32_t midpoint32_t = calculate_midpoint32_t(first, last);
    if (sa->cf(sa->array[first], sa->array[midpoint32_t]) < 0) {
      median = midpoint32_t;
    }
    else {
      smallest = midpoint32_t;
    }
    if (sa->cf(sa->array[last], sa->array[smallest]) < 0) {
      median = smallest;
    }
    else if (sa->cf(sa->array[last], sa->array[median]) < 0) {
      median = last;
    }
    return median;
  }
  else {
    return first;
  }
}
static int32_t choose_pivot(sortable_array *sa, int32_t first, int32_t last) {
  return choose_median_of_three(sa, first, last);
}
static void swap(sortable_array *sa, int32_t i, int32_t j) {
  if (i != j) {
    void *temp = sa->array[i];
    sa->array[i] = sa->array[j];
    sa->array[j] = temp;
  }
}
static int32_t partition(sortable_array *sa, int32_t first, int32_t last) {
  int32_t pivot = choose_pivot(sa, first, last);
  if (pivot != first) {
    swap(sa, first, pivot);
  }
  int32_t i = first + 1, j = first + 1;
  while (j <= last) {
    if (sa->cf(sa->array[j], sa->array[first]) < 0) {
      swap(sa, i, j);
      i++;
    }
    j++;
  }
  pivot = i - 1;
  swap(sa, first, pivot);
  return pivot;
}
