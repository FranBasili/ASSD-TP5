import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as ss

deltaf0 = 100

f0 = 1e3; fs = 10e3; ws= 2*np.pi * fs
w0 = 2* np.pi * f0/fs
deltaw =  deltaf0 / f0
R= (2-deltaw) / 2

znum = [1, 0, -1]
pden = [1, -2*R*np.cos(w0), R**2]
w, h = ss.freqz(znum, pden, worN=5000, fs= ws)

plt.plot(w / (2*np.pi), 20*np.log10(np.abs(h)), color='blue', label='$H(e^{j 2 \pi f T_s})$')
plt.legend()
plt.xlabel("frecuencia [Hz]")
plt.ylabel("Amplitud [dB]")
plt.grid('both')
plt.show()
