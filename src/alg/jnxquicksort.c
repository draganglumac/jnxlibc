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

void kwiksort(sortable_array *sa, int first, int last); 
int choose_pivot(sortable_array *sa, int first, int last);
void swap(sortable_array *sa, int i, int j);
int partition(sortable_array *sa, int first, int last);

void jnx_quicksort(void **array, int size, compare_function cf) {
	if (array == 0 || size <= 0) {
		return;
	}
	sortable_array sa;
	sa.array = array;
	sa.cf = cf;
	kwiksort(&sa, 0, size - 1);
}
void kwiksort(sortable_array *sa, int first, int last) {
	if (first >= last) {
		return;
	}
	int pivot = partition(sa, first, last);
	kwiksort(sa, first, pivot - 1);
	kwiksort(sa, pivot + 1, last);
}
int calculate_midpoint(int first, int last) {
	int size = last - first + 1;
	if (size % 2 == 0) {
		return first + size/2 - 1;
	}
	else {
		return first + size/2;
	}
}
int choose_median_of_three(sortable_array *sa, int first, int last) {
	if (last - first > 1) {
		int smallest = first, median = first;
		int midpoint = calculate_midpoint(first, last);
		if (sa->cf(sa->array[first], sa->array[midpoint]) < 0) {
			median = midpoint;
		}
		else {
			smallest = midpoint;
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
int choose_pivot(sortable_array *sa, int first, int last) {
	return choose_median_of_three(sa, first, last);
}
void swap(sortable_array *sa, int i, int j) {
	if (i != j) {
		void *temp = sa->array[i];
		sa->array[i] = sa->array[j];
		sa->array[j] = temp;
	}
}
int partition(sortable_array *sa, int first, int last) {
	int pivot = choose_pivot(sa, first, last);
	if (pivot != first) {
		swap(sa, first, pivot);
	}
	int i = first + 1, j = first + 1;
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
