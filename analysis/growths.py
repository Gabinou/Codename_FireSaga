
import os
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

# --- GROWTH UNIT STATISTICAL ANALYSIS ---

# TODO: Total 	growth compared for all characters
# TODO: STR 	growth compared for all characters
# TODO: MAG 	growth compared for all characters
# TODO: DEX 	growth compared for all characters
# TODO: AGI 	growth compared for all characters
# TODO: LUCK 	growth compared for all characters
# TODO: FTH 	growth compared for all characters
# TODO: DEF 	growth compared for all characters
# TODO: RES 	growth compared for all characters
# TODO: MV 		growth compared for all characters
# TODO: CON 	growth compared for all characters
# TODO: PROF 	growth compared for all characters


