/*© 2021 Aevice Health Pte Ltd. All rights reserved.*/
/*
 * sorting_algo.c
 *
 *  Created on: 11 Mar 2020
 *      Author: aiman
 */

#include "initialisations.h"

void swapf(float32_t *xp, float32_t *yp);
void swapf(float32_t *xp, float32_t *yp)
{
    float32_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swapi(uint32_t *xp, uint32_t *yp);
void swapi(uint32_t *xp, uint32_t *yp)
{
    uint32_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSortf(float32_t arr[], uint32_t n);
void selectionSortf(float32_t arr[], uint32_t n)
{
    uint32_t i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] < arr[min_idx])
            min_idx = j;

        // Swap the found minimum element with the first element
        swapf(&arr[min_idx], &arr[i]);
    }
}

void selectionSorti(uint32_t arr[], uint32_t n);
void selectionSorti(uint32_t arr[], uint32_t n)
{
    uint32_t i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] < arr[min_idx])
            min_idx = j;

        // Swap the found minimum element with the first element
        swapi(&arr[min_idx], &arr[i]);
    }
}
