#ifndef __CONFIG_H__
#define __CONFIG_H__

#define SIZE  32 
#define ROI   5 /// must be at least 3
#define COUNT 50

void threshold(double Z[ROI][ROI], double theshold, uint32_t x, uint32_t y, double *Z_thresh);
void maximum(double arr[SIZE][SIZE], double* max, uint32_t *locx, uint32_t *locy);
void centroid(double Z[ROI][ROI], uint32_t x, uint32_t y, double* cen_x, double* cen_y);
void create_roi(double Z[SIZE][SIZE], uint32_t x, uint32_t y, double *roi);

#endif
