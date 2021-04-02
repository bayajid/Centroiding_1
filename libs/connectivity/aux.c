#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "aux.h"
/************** Small ROI  **************/
/**	@brief Construct a small ROI around the brightest pixel.
    @param INPUT:
		2-D array[][]
	@param OUTPUT:
		2-D ROI[][]
*/
void create_roi(double Z[SIZE][SIZE], uint32_t x, uint32_t y, double *roi)
{
	uint32_t i = 0, j = 0;
	for ( i = x-(ROI-1)/2; i <= x+(ROI-1)/2; i++){
	   for ( j = y-(ROI-1)/2; j <= y+(ROI-1)/2; j++){
		   *(roi+ROI*i+j) = Z[i][j];
		    //printf("ROI : %lf, %u,%u\n",*(roi+ROI*i+j),i,j );
	   }
   }
}

/************ Thresholding fucntion*************/
/** @brief	Mask the input array. Any value lower than threshold is set to zero.
	@param INPUT:
		2-D array[][]
		threshold value
	@param OUTPUT:
		Thresholded 2-D array[][]
*/
void threshold(double Z[ROI][ROI], double threshold, uint32_t x, uint32_t y, double *Z_thresh)
{
	uint32_t i, j;

	for ( i = x-(ROI-1)/2; i <= x+(ROI-1)/2; i++){
	   for ( j = y-(ROI-1)/2; j <= y+(ROI-1)/2; j++){
			if (Z[i][j] < threshold){
				Z[i][j] = 0;
			}
			else{
				*(Z_thresh+ ROI*i+j) = Z[i][j];
			}
		}
	}
}

/************* maximum of a 2D array***************/
/** @brief Maxima search routine. Runs over the whole pixel range.
* Returns the location of the pixel with maximum intensity and the intesity value.
	@param INPUT:
		2D array[][]
	@param OUTPUT:
	 	Maxuimum value of the 2-D Array
		location x,y of the maximum value
*/
void maximum(double arr[SIZE][SIZE], double* max, uint32_t *locx, uint32_t *locy)
{
	for (uint32_t i = 0; i< SIZE; i++){
		for (uint32_t j = 0; j< SIZE; j++){
			if (arr[i][j] > *max){
				*max = arr[i][j];
				*locx = i;
				*locy = j;
			}
		}
	}
}
/************** Centroiding function **************/
/**	@brief Main centroid algorithm. Performs on small region of interest (ROI).
    @param INPUT:
		2-D array[][]
	@param OUTPUT:
		Centroids: cx, cy
*/
void centroid(double Z[ROI][ROI], uint32_t x, uint32_t y, double* cen_x, double* cen_y)
{
	double pixel_val = 0;
	uint32_t i,  j;
	for ( i = x-(ROI-1)/2; i <= x+(ROI-1)/2; i++){
	   for ( j = y-(ROI-1)/2; j <= y+(ROI-1)/2; j++){

		   if( Z[i][j] != 0) {
				//printf("cc: [%u][%u] %3.3lf\n", i,j, Z[i][j]);
				*cen_x += i*Z[i][j];
				*cen_y += j*Z[i][j];
				pixel_val +=  Z[i][j];
			}
		}
	}
	*cen_x /= pixel_val;
	*cen_y /= pixel_val;
}
