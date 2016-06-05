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

static void kwiksort(sortable_array *sa, jnx_int32 first, jnx_int32 last); 
static jnx_int32 choose_pivot(sortable_array *sa, jnx_int32 first, jnx_int32 last);
static void swap(sortable_array *sa, jnx_int32 i, jnx_int32 j);
static jnx_int32 partition(sortable_array *sa, jnx_int32 first, jnx_int32 last);

void jnx_quicksort(void **array, jnx_int32 size, compare_function cf) {
  if (array == 0 || size <= 0) {
    return;
  }
  sortable_array sa;
  sa.array = array;
  sa.cf = cf;
  kwiksort(&sa, 0, size - 1);
}
static void kwiksort(sortable_array *sa, jnx_int32 first, jnx_int32 last) {
  if (first >= last) {
    return;
  }
  jnx_int32 pivot = partition(sa, first, last);
  kwiksort(sa, first, pivot - 1);
  kwiksort(sa, pivot + 1, last);
}
static jnx_int32 calculate_midpojnx_int32(jnx_int32 first, jnx_int32 last) {
  jnx_int32 size = last - first + 1;
  if (size % 2 == 0) {
    return first + size/2 - 1;
  }
  else {
    return first + size/2;
  }
}
static jnx_int32 choose_median_of_three(sortable_array *sa, jnx_int32 first, jnx_int32 last) {
  if (last - first > 1) {
    jnx_int32 smallest = first, median = first;
    jnx_int32 midpojnx_int32 = calculate_midpojnx_int32(first, last);
    if (sa->cf(sa->array[first], sa->array[midpojnx_int32]) < 0) {
      median = midpojnx_int32;
    }
    else {
      smallest = midpojnx_int32;
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
static jnx_int32 choose_pivot(sortable_array *sa, jnx_int32 first, jnx_int32 last) {
  return choose_median_of_three(sa, first, last);
}
static void swap(sortable_array *sa, jnx_int32 i, jnx_int32 j) {
  if (i != j) {
    void *temp = sa->array[i];
    sa->array[i] = sa->array[j];
    sa->array[j] = temp;
  }
}
static jnx_int32 partition(sortable_array *sa, jnx_int32 first, jnx_int32 last) {
  jnx_int32 pivot = choose_pivot(sa, first, last);
  if (pivot != first) {
    swap(sa, first, pivot);
  }
  jnx_int32 i = first + 1, j = first + 1;
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
