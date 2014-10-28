/*
 * =====================================================================================
 *
 *       Filename:  quicksort.c
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

void kwiksort(void **array, int first, int last, compare_function cf); 
int choose_pivot(void **array, int first, int last, compare_function cf);
void swap(void **array, int i, int j);
int partition(void **array, int first, int last, compare_function cf);

void quicksort(void **array, int size, compare_function cf) {
	if (array == 0 || size <= 0) {
		return;
	}
	kwiksort(array, 0, size - 1, cf);
}
void kwiksort(void **array, int first, int last, compare_function cf) {
	if (first >= last) {
		return;
	}
	int pivot = partition(array, first, last, cf);
	kwiksort(array, first, pivot - 1, cf);
	kwiksort(array, pivot + 1, last, cf);
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
int choose_median_of_three(void **array, int first, int last, compare_function cf) {
	if (last - first > 1) {
		int smallest = first, median = first;
		int midpoint = calculate_midpoint(first, last);
		if (cf(array[first], array[midpoint]) < 0) {
			median = midpoint;
		}
		else {
			smallest = midpoint;
		}
		if (cf(array[last], array[smallest]) < 0) {
			median = smallest;
		}
		else if (cf(array[last], array[median]) < 0) {
			median = last;
		}
		return median;
	}
	else {
		return first;
	}
}
int choose_pivot(void **array, int first, int last, compare_function cf) {
	return choose_median_of_three(array, first, last, cf);
}
void swap(void **array, int i, int j) {
	if (i != j) {
		void *temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}
int partition(void **array, int first, int last, compare_function cf) {
	int pivot = choose_pivot(array, first, last, cf);
	if (pivot != first) {
		swap(array, first, pivot);
	}
	int i = first + 1, j = first + 1;
	while (j <= last) {
		if (cf(array[j], array[first]) < 0) {
			swap(array, i, j);
			i++;
		}
		j++;
	}
	pivot = i - 1;
	swap(array, first, pivot);
	return pivot;
}
