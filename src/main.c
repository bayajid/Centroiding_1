/*  CENTROIDING ALGORITHM
* 	Author: Shaker Bayajid KHAN
*   Date: March 01, 2021
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "connectivity/aux.h"

// #include "time.h"
// #define SIZE 32
// #define ROI 3 /// must be at least 3
// #define COUNT 50


/** @brief
* This is the main() program for calculating centroid of an image.
* The program reads image data as .txt format. Then the signal is seperated
* from the average background and searches for the brightest spot.
* An user defined ROI is created around the brightest spot and a threshold value is applied.
* All the pixels below the threshold is set to zero and over the rest of the pixels
* inside the ROI centroid function is applied to locate the center.
* Finally the data is saved in a .txt file for analysis.

* @retval Centroids.
* @retval Error in Pixels.
*
*/
int main()
{	uint32_t i, j, xval, yval;
	uint8_t loopcounter;
	FILE *fptr, *fptr1, *qfile;

	//clock_t start_t, end_t, total_t;

	char name_buffer1[512];
	char name_buffer2[512];
	char name_buffer3[512];


	/* Dynamic allocation of 2D array*/
	double(*intensity_data)[SIZE] = malloc(sizeof *intensity_data*SIZE);
	double(*only_signal)[SIZE] = malloc(sizeof *only_signal*SIZE);

	double(*thresholded_val)[ROI] = malloc(sizeof *thresholded_val*ROI);
	double(*small_roi)[ROI] = malloc(sizeof *small_roi*ROI);

	const char* in_directory = "data/input/";
	const char* out_directory = "data/output/";
	const char* fileName = "Python_result-";
	const char* fileType = ".txt";

	const char* outfile1 = "Result";
	const char* outfile2 = "plotting-";
	sprintf(name_buffer2,"%s%s%s",out_directory,outfile1,fileType);


	/* Read spot data file from python generated .txt file*/
	for(loopcounter =0; loopcounter<COUNT; loopcounter++){
    //Print to character buffer
    	sprintf(name_buffer1,"%s%s%u%s",in_directory,fileName,loopcounter,fileType);

		sprintf(name_buffer3,"%s%s%u%s",out_directory,outfile2,loopcounter,fileType);
    	//fptr = fopen(name_buffer,"r");
		//fprintf(f, "hello world");
		//fclose(f);
	printf("%s\n\n",name_buffer1);

	/* Initialise variables */
	double Max_intensity = 0, cx = 0, cy = 0, sum_intensity = 0, sum_only_signal = 0;
	double avg = 0, avg_signal = 0;
	double Error[2], Error_r = 0;
	double selec_Thr = 0;


	if ((fptr = fopen(name_buffer1,"r"))==NULL){
		printf("Error!opening image data file.\n");
		exit(1);
	}
	/* Open file to write results */
	if ((fptr1 = fopen(name_buffer2,"a"))==NULL){
		printf("Error!opening file: %s \n",outfile1);
		exit(1);
	}
	if ((qfile = fopen(name_buffer3,"w"))==NULL){
		printf("Error!opening file: %s\n",outfile2);
		exit(1);
	}
	/* Array initialisation */
	for (i = 0; i < SIZE; i++){
		for (j = 0; j < SIZE; j++){
			intensity_data[i][j] = 0;
			only_signal[i][j] = 0;
		}
	}
	for (i = 0; i < ROI; i++){
		for (j = 0; j < ROI; j++){
			thresholded_val[i][j] = 0;
			small_roi[i][j] = 0;
		}
	}

	/* Fill the data in a 2D array*/
	for (i = 0; i < SIZE; i++){
		for (j = 0; j < SIZE; j++){
			if (fscanf(fptr, "%lf,", &intensity_data[i][j]) !=1){
				printf("Error! Reading file.\n");
				exit(1);
			}
			sum_intensity += intensity_data[i][j];
		}
	}

	avg = sum_intensity/(SIZE*SIZE);

	for (i = 0; i < SIZE; i++){
		for (j = 0; j < SIZE; j++){
			if ((intensity_data[i][j]-avg) < 0){
				only_signal[i][j] = 0;
			}
			else{
				only_signal[i][j] = intensity_data[i][j] - avg;
				sum_only_signal += only_signal[i][j];
			}
		}
	}
	avg_signal = sum_only_signal/(SIZE*SIZE);
	printf("AVG: %lf\n",avg_signal );
	selec_Thr = avg_signal + 3*sqrt(avg_signal);
	printf("Threshold: %3.3lf\n",selec_Thr);

	/* Find the maximum intesity value */
	maximum(only_signal, &Max_intensity, &xval, &yval) ;
	for (i = xval-(ROI-1)/2; i <= xval+(ROI-1)/2; i++){
		for (j = yval-(ROI-1)/2; j <= yval+(ROI-1)/2; j++){
			thresholded_val[i][j] = 0;
			small_roi[i][j] = 0;
		}
	}

	/* print the maximum value to check */
	printf("Max. Intensity: %3.3e\tCen_location:[%u][%u]\n",Max_intensity, xval, yval);

	/* create a small ROI  */
	create_roi(only_signal, xval, yval, &small_roi[0][0]);

	/* Mask the data by thresholding.Any value below the selected threshold is set to Zero*/
	threshold(small_roi, selec_Thr, xval, yval, &thresholded_val[0][0]);

	centroid(thresholded_val, xval, yval, &cx, &cy);
	//end_t = clock();
	//total_t = (double)(end_t - start_t) /CLOCKS_PER_SEC;

	/* Considering the spot is at the very centre of the frame,
	(if not, check the python file to see where is the exact spot
	location and caculate the error accordingly) */

	Error[0] = cx - SIZE/2;
	Error[1] = cy - SIZE/2;
	Error_r = sqrt(Error[0]*Error[0]+Error[1]*Error[1]);

	printf("\nError values[px]: x:%3.3e, y:%3.3e\nError_r[px]: %3.3lf\nCentroids: \n%lf\n%lf",Error[0],Error[1],Error_r,cx,cy );

	/* print centroid data to file */
	fprintf(fptr1,"%lf,%lf\n",Error[0],Error[1]);

	for (i = xval-(ROI-1)/2; i <= xval+(ROI-1)/2; i++){
		for (j = yval-(ROI-1)/2; j <= yval+(ROI-1)/2; j++){
		   fprintf(qfile, "%lf,\n",thresholded_val[i][j]);
		}
	}
	fprintf(qfile, "%u,\n%u,\n%u \n%lf",xval-(ROI-1)/2,yval-(ROI-1)/2,ROI,Error_r);

	/* Close files */
	fclose(fptr);
	fclose(fptr1);
	fclose(qfile);

	printf("\nloopcounter: %u\n\n",loopcounter );
	}

	/* free the previously allocated array */
	free(intensity_data);
	free(only_signal);
	free(thresholded_val);
	free(small_roi);

	return 0;
}
