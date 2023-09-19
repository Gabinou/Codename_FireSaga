import numpy as np
import scipy.stats
import matplotlib.pyplot as plt

cd
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

	legends.append("Growths: " + str(growth) + " Levels: " + str(levels))
plt.legend(legends)
plt.show()