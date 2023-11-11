
import os
import sys
import json
import numpy as np
import scipy.stats
import matplotlib.pyplot as plt

source_dir = os.path.dirname(os.path.abspath(__file__))

# --- GROWTH META STATISTICAL ANALYSIS ---
# Growth PMFs using Binomial distribution
growths = [0.2, 0.3, 0.4, 0.5, 0.6]
levels = 10
legends = []
for growth in growths:
	rv = scipy.stats.binom(levels, growth)
	x = np.linspace(0,levels,levels+1)
	# plt.plot(x, rv.pmf(x), "k.")
	plt.plot(x, rv.pmf(x))
	# plt.ylim([-0.02, 1.05])
	plt.xlabel("Total Growth [stat]")
	plt.ylabel("Probability")

	legends.append(f"{growth}, {levels} lvls")
plt.legend(legends)
# plt.show()
plt.savefig(os.path.join(source_dir, "expected_growth.png"))
plt.close()

# --- GROWTH UNIT STATISTICAL ANALYSIS ---
stats = ["HP", "STR", "MAG","DEX","AGI","LUCK","FTH","DEF","RES","MV","CON","PROF"]
units = ["Erwin", "Kiara", "Silou", "Lauzon", "Pirou", "Rayan", "Sebastian", "Servil", "Perignon", "Michael"]

stats_num = 12
growths_matrix = np.array([]).reshape((0,stats_num))
for unit in units:
	filename = os.path.join(source_dir,"..", "units", unit + ".json")
	with open(filename, 'r') as f:
	    array = json.load(f)
	    growths = array["Unit"]["Growths"]
	growths_arr = np.array(list(growths.values())).reshape(1, -1)
	growths_matrix = np.vstack((growths_matrix, growths_arr))
total_growths = np.sum(growths_matrix, axis=1) 

# Total growth compared for all characters
plt.plot(np.ravel(units), np.ravel(total_growths), "ko")
plt.ylabel("Total growths [%]")
plt.xticks(rotation=55)
plt.tight_layout()
# plt.show()
plt.savefig(os.path.join(source_dir, "units_total_growths.png"))
plt.close()

# Stat growth compared for all characters
for i in np.arange(len(stats)):
	stat = stats[i]
	plt.plot(np.ravel(units), np.ravel(growths_matrix[:,i]), "ko")
	plt.ylabel(stat + " Growths [%]")
	plt.xticks(rotation=55)
	plt.tight_layout()
	# plt.show()
	plt.savefig(os.path.join(source_dir, f"units_{stat}_growths.png"))
	plt.close()
