import numpy as np
import scipy.stats
import sys
import matplotlib.pyplot as plt

chapter_num = 20
levels = 40
# GUARD,    LIBRARIAN,    ASSISTANT,    COOK,    SCRIBE,    STABLEHAND,    CLERGYMAN,    STORAGEMASTER,    MAGICRESEARCHER,
bonus_exp = np.array([50, 10, 10, 20, 20, 20, 20, 10, 50], dtype=int)
jobs_max = np.array([4, 2, 1, 3, 2, 2, 2, 2, 2], dtype=int)
party_size = 20
max_party_size = 40

total_guard_levels = int(bonus_exp[0] * chapter_num / 100)
print(party_size, "man party", chapter_num, "chapters")
print(total_guard_levels, "lvls for one guard")



current_jobs_max = np.ceil(party_size / max_party_size * jobs_max)
print(current_jobs_max)
current_employed = int(np.sum(current_jobs_max)) 
bonus_exp_per_chapter = current_jobs_max * bonus_exp
total_bonus_exp_per_chapter = int(np.sum(bonus_exp_per_chapter))

canfight_bonus_exp_per_chapter = int(np.sum(bonus_exp_per_chapter[1:-1]))
cantfight_bonus_exp_per_chapter = int(bonus_exp_per_chapter[0] + bonus_exp_per_chapter[-1])
canfight_employees = int(np.sum(current_jobs_max[1:-1]))
cantfight_employees = int(current_jobs_max[0] + current_jobs_max[-1])
print(current_employed, "employees")
print(total_bonus_exp_per_chapter, "job exp per chapter")
levels_per_chapter = total_bonus_exp_per_chapter / 100
canfight_levels_per_chapter = canfight_bonus_exp_per_chapter / 100 
cantfight_levels_per_chapter = cantfight_bonus_exp_per_chapter / 100 
print(levels_per_chapter, "lvls per chapter spread between", current_employed, "employees")
print(canfight_levels_per_chapter, "lvls per chapter spread between", canfight_employees, "fighters")
print(cantfight_levels_per_chapter, "lvls per chapter spread between", cantfight_employees, "non-fighters")

total_levels_jobs_exp = levels_per_chapter * chapter_num
canfight_levels = canfight_levels_per_chapter * chapter_num
cantfight_levels = cantfight_levels_per_chapter * chapter_num
print(total_levels_jobs_exp, "total job lvls spread between", current_employed, "employees")
print(canfight_levels, "lvls spread between", canfight_employees, "fighters,", canfight_levels/canfight_employees, "per fighter")
print(cantfight_levels, "lvls spread between", cantfight_employees, "non-fighters,", cantfight_levels/cantfight_employees, "per non-fighter")
total_levels_jobs_exp_per_employee = total_levels_jobs_exp / current_employed
print(np.round(total_levels_jobs_exp_per_employee), "levels per employee")
print(levels, "total lvls per employees")
print(levels * current_employed, "total lvls for all employees")

print("Conclusion:")
print("\tI prefer to SPREAD the bonus exp. Don't like to be able to dump EVERYTHING into one guy.")
print("\t50 EXP for non-fighters is good. 2 Chapters per level maximum. \n\tScrubs can get a couple nice levels, but not enough to max out")
print("\t25 EXP maximum for employed fighters is good. 4 Chapters per level MAXIMUM. \n\t10-20 EXP is good. Just a simple bonus.")