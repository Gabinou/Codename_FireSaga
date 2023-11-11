
import numpy as np
import matplotlib.pyplot as plt
import os

# Getting RNGs 
source_dir = os.path.dirname(os.path.abspath(__file__))
rng_csv = os.path.abspath(os.path.join(source_dir, "..", "build", "RNG.csv"))
rngs = np.loadtxt(rng_csv)

# Printing histogram of RNGs
plt.hist(rngs, bins = 100, density=True)
plt.xlabel("RN [0, 99]")
plt.ylabel("Number of occurences")
# plt.show()
plt.savefig(os.path.join(source_dir, "RNG_density.png"))
