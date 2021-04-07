# Centroiding
## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)

## General info
This project is to address the performance of centroiding accuracy by simulating an image frame with a spot and performing centroid calculation. To be more realistic, background noise is added so the image output from the Python program will be representative to an image taken from the selected camera.  

(1) <img src= "images/Screenshot%202021-04-05%20at%2015.14.29.png" width= 300 Height = 150> 
(2) <img src= "images/Screenshot%202021-04-05%20at%2020.04.24.png" width= 300 Height = 150>


(3) <img src= "images/Screenshot%202021-04-05%20at%2015.13.23.png" width= 300 Height = 150>


The image (1) shows an example of the airy spot creation, image(2) shows the simulated image as one possible solution of a random normal with mean equal to spot incident signal and standard deviation of the noise. Image (3) shows the resultant when the background is averaged and subtracted from image (2) to obtain only the spot.
	
## Technologies
Project is created with:
* Spot image creation: Python version: 3.xx
* Centroiding : C

	
## Setup
To run this project, simply run the make clean to clean previous data and then run make:

```
$ make clean
$ make

```
Note: This will first run the Python file for image creation and will prompt user to input camera. 3 cameras has been listed and the option (1) will select a CMOS camera and the 2 others are InGAas camera. The wavelength and camera settings will change accordingly.
