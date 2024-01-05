#ifndef UNIT_STATS_H
#define UNIT_STATS_H

/* --- Unit stats --- */
struct Unit_stats Unit_stats_plus(     struct Unit_stats stats1, struct Unit_stats stats2);
struct Unit_stats Unit_stats_minus(    struct Unit_stats stats1, struct Unit_stats stats2);
struct Unit_stats Unit_stats_div_cst(  struct Unit_stats stats,  int cst);
struct Unit_stats Unit_stats_plus_cst( struct Unit_stats stats1, int cst);
struct Unit_stats Unit_stats_minus_cst(struct Unit_stats stats,  int cst);

#endif /* UNIT_STATS_H */
