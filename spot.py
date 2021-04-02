import numpy as np
import math
import matplotlib.pyplot as plt
from scipy.special import jv
from prysm import Pupil, PSF, MTF
from prysm.psf import airydisk
from prysm.otf import diffraction_limited_mtf
from prysm.psf import FIRST_AIRY_ENCIRCLED
from PIL import Image
import PIL
from matplotlib.colors import PowerNorm


SIZE = 32
RunTime = 50

wavelength = 528e-9 # m 

# in SI units
distance = 100e-3 # m
aperture_diameter = 50e-3 # m 
V_light = 299792458 
plank = 6.626E-34
FPS = 300
int_time = 1/FPS
mag = 6

conv_gain = 1#0.11 			# Unit DN/e
DS = 70  					# Dark current/signal [e/s]
QE = 0.43					# Quantum Efficiency
RO = 13 * conv_gain			# Readout noise/Temporal [DN]


BG_dBm = -104.6			    # in dBm
BG = 0.001*10**(BG_dBm/10)  # Background signal

Beacon_dBm = -100.2
Beacon = 0.001*10**(Beacon_dBm/10)

epd = aperture_diameter*1e3  # in mm
efl = mag*distance*1e3         # in mm
fno = efl / epd

p = Pupil(wavelength=0.528, dia=epd, samples=(SIZE//2))
psf = PSF.from_pupil(p, efl)
u, sx = psf.slices().x
v, sy = psf.slices().y

datafile = psf.data

x = np.linspace(-88,88,176)
y = np.linspace(-88,88,176)
X,Y = np.meshgrid(x,y)

r = np.sqrt(X**2+Y**2)

def signaltonoise(a, axis=0, ddof=0):
	a = np.asanyarray(a)
	m = a.mean(axis)
	sd = a.std(axis=axis, ddof=ddof)
	return np.where(sd == 0, 0, m/sd)


analytical_psf = airydisk(r*5.5, fno, wavelength*1e6)
Intensity = analytical_psf#psf.data


N_electron = (BG)*wavelength*QE/(plank*V_light)

def signal_on_pix(signal):
	N_electron = (signal)*wavelength*QE/(plank*V_light)
	if ('signal'== BG):
		return (N_electron+DS)*int_time   # multiply by conv_gain to be in DN
	else:
		return (N_electron)*int_time

BG_signal_EL = signal_on_pix(signal = BG)
BG_signal = conv_gain*BG_signal_EL
Beacon_signal_EL = signal_on_pix(signal = Beacon)
Beacon_signal = conv_gain*Beacon_signal_EL
#new_beacon_signal = signal_on_pix(signal = analytical_psf)

print("DS + BG signal[DN]: ", BG_signal)
print("\nMax.Beacon signal[DN]: ", np.max(Beacon_signal))

percent_beacon = (100*psf.data)/(np.sum(sum(psf.data)))
signal_beacon = percent_beacon*Beacon_signal/100
noise_beacon = np.sqrt(signal_beacon)
Brightest_pix = np.max(signal_beacon) 


signal_backG = np.zeros((SIZE,SIZE))
noise_backG = np.zeros((SIZE,SIZE))
RO_Z = np.zeros((SIZE,SIZE))
saturation = np.zeros((SIZE,SIZE))
only_signal = np.zeros((SIZE,SIZE))
for i in range(SIZE):
	for j in range(SIZE):
		signal_backG [i][j] =  BG_signal	
		noise_backG [i][j] = np.sqrt(BG_signal)
		RO_Z[i][j] = RO


Total_signal = signal_beacon + signal_backG
Total_noise = 3 * np.sqrt(RO_Z**2 + noise_backG**2 + noise_beacon**2)
SNR_BR_px = Brightest_pix /np.sqrt(Brightest_pix + BG_signal + RO_Z**2)
SNR_split = (Brightest_pix/4) /np.sqrt((Brightest_pix/4) + BG_signal + RO_Z**2)

print("SNR_BR_px:", np.max(SNR_BR_px))

for i in range(RunTime):
	Result = np.random.normal(Total_signal,Total_noise/3 )
	filename = "{}-{}.{}".format("data/input/Python_result",i,"txt")
	np.savetxt(filename,Result,delimiter=",")
	snr_val = np.mean(Result)/np.std(Result)
	print("snr_val:",np.max(snr_val))

for i in range(SIZE):
	for j in range(SIZE):
		if (Result[i][j] >= 13500*conv_gain):
			saturation[i][j] = Result[i][j]
		else:
			saturation[i][j] = 0

avg = np.average(Result)
for i in range(SIZE):
	for j in range(SIZE):
		if (Result[i][j] - avg > 0):
			only_signal[i][j] = Result[i][j] -avg
		else:
			only_signal[i][j] = 0

# Create figure

x = np.linspace(-SIZE/2,SIZE/2,SIZE)
y = np.linspace(-SIZE/2,SIZE/2,SIZE)
XX,YY = np.meshgrid(x,y)

## figure 1
# fig1, ((ax1, ax4)) = plt.subplots(1, 2)
# fig1.suptitle('Spot generation')
# plt.grid()
# im = ax1.imshow(signal_beacon,cmap='viridis')
# ax1.set_title("Signal")
# ax4.set_title("Airy spot")
# psf.plot2d(fig = fig1,ax = ax4,xlim=32*1.22*psf.wavelength*FIRST_AIRY_ENCIRCLED,
#           power=1/2,
#           interpolation='lanczos',
#           show_axlabels=True,
#           show_colorbar=True,cmap='viridis')

## figure 2
fig2, ((ax2,ax3)) = plt.subplots(1,2)
fig2.suptitle('Frame rate [fps] ={:03.2f}'.format(FPS))
ax2.set_title(r'Background, Beacon')#, max.SNR{:03.2f}'.format(np.max(signaltonoise(signal_beacon,axis=0,ddof=0))))
ax2.plot(x,Result[:,16],'-g')
ax2.bar(x,Total_signal[:,16], label='Beacon signal')
ax2.bar(x,signal_backG[:,16], label='Background+DC')
ax2.set_xlabel('Pixels')
ax2.set_ylabel('DN')
ax2.legend(loc='upper left') 
ax2.errorbar(x,Total_signal[:,16], yerr =Total_noise[:,16], 
			marker='o', ecolor='gray',markersize=2,linestyle='none')

im2 = ax3.imshow(Result,vmin = 10, cmap = 'viridis')
ax3.set_title("Signal /w noise")

## figure 3
fig3,(ax1,ax2) = plt.subplots(1,2)
ax1.imshow(saturation,vmax = 13500*conv_gain,cmap = 'viridis')
ax2.imshow(only_signal)



# np.savetxt('Python_result_only_signal.txt', only_signal,delimiter=",")


plt.show()


