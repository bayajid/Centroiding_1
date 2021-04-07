# Centroiding
## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)

## General info
This project is to address the performance of centroiding accuracy by simulating an image frame with a spot and performing centroid calculation. To be more realistic, background noise is added so the image output from the Python program will be representative to an image taken from the selected camera.  

	
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
