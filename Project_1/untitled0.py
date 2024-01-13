# -*- coding: utf-8 -*-
"""
Created on Sun Dec 10 15:50:41 2023

@author: BilgeKaan
"""

import matplotlib.pyplot as plt
import numpy as np

# Sample data (replace this with your actual data)
frequencies = [1450000, 1510000, 1550000, 1580000, 1600000, 1900000, 1930000, 1950000, 
               1970000, 2000000, 2350000, 2380000, 2420000, 2480000]
amplitudes = [62/2600, 78/3000, 100/3320, 116/3640, 136/3820, 256/492, 280/416, 296/388, 
              292/424, 252/508, 162/4320, 146/4080, 116/3760, 92/3440]
frequencies = np.array(frequencies)
amplitudes = np.array(amplitudes)
amplitudes_dB = 20 * np.log10(amplitudes)

# Create subplots
fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True, figsize=(8, 6))

# Plotting amplitude response in dB on logarithmic scale
ax1.semilogx(frequencies, amplitudes_dB, marker='o', linestyle='-', color='b')
ax1.set_ylabel('Amplitude (dB)')

# Plotting phase response on logarithmic scale
ax2.semilogx(frequencies, marker='o', linestyle='-', color='r')
ax2.set_xlabel('Frequency (Hz)')
ax2.set_ylabel('Phase (degrees)')

# Adding titles
ax1.set_title('Bandpass Filter Response (Amplitude in dB)')
ax2.set_title('Bandpass Filter Phase Response')

# Adjust layout for better spacing
plt.tight_layout()

# Displaying the graph
plt.show()
