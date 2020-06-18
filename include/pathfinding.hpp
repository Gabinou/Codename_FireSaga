#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <string>
#include <vector>
#include <cmath>
#include <cstdint>
// #include "linalg.hpp"
#include <SDL2/SDL.h>

extern int32_t h_manhattan(int32_t start[], int32_t end[]);
extern int32_t h_manhattan(std::vector<int32_t> start, std::vector<int32_t> end);
extern int32_t h_manhattan(std::vector<int32_t> start, int32_t end[]);
extern int32_t h_manhattan(int32_t start, std::vector<int32_t> end[]);
extern int64_t h_euclidean(int32_t start[], int32_t end[]);

extern std::vector<std::vector<int32_t>> A_star(std::vector<std::vector<int32_t>> map, int32_t start[], int32_t end[], std::string mode = "matrix");

template <typename T> extern std::vector<std::vector<T>> attackmap(std::vector<std::vector<T>> movemap, uint32_t start[], uint32_t move, uint8_t range[2], std::string mode = "matrix") {
    // Using the movemap to compute all attackable tiles.
    // EXCLUDING moveable tiles.
    std::vector<std::vector<T>> attackmap;
    std::vector<std::vector<T>> movelist;
    std::vector<T> temp_point32_t = {0, 0};
    T tempx, tempy;

    movelist = matrix2list(movemap);

    if (mode == "matrix") {
        attackmap = movemap;

        for (int32_t i = 0; i < attackmap.size(); i++) {
            std::fill(attackmap[i].begin(), attackmap[i].end(), 0);
        }
    }

    for (int32_t i = 0; i < movelist.size(); i++) {
        for (int32_t rangex = 0; rangex <= range[1]; rangex++) {
            int32_t miny = std::max(0, range[0] - rangex);
            int32_t maxy = std::max(range[1] - rangex, 0);

            for (int32_t rangey = miny; rangey <= maxy; rangey++) {
                tempx = std::min(movelist[i][1] + rangex, (int32_t)(movemap[0].size() - 1));
                tempy = std::min(movelist[i][0] + rangey, (int32_t)(movemap.size() - 1));

                if ((tempx < (int32_t)movemap[0].size()) && (tempy < (int32_t)movemap.size())) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point32_t = {tempx, tempy};
                            attackmap.push_back(temp_point32_t);
                        }

                        if (mode == "matrix") {
                            attackmap[tempy][tempx] = 1;
                        }
                    }
                }

                tempx = std::max(movelist[i][1] - rangex, 0);
                tempy = std::min(movelist[i][0] + rangey, (int32_t)(movemap.size() - 1));

                if ((tempx >= 0) && (tempy < (int32_t)movemap.size())) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point32_t = {tempx, tempy};
                            attackmap.push_back(temp_point32_t);
                        }

                        if (mode == "matrix") {
                            attackmap[tempy][tempx] = 1;
                        }
                    }
                }

                tempx = std::min(movelist[i][1] + rangex, (int32_t)(movemap[0].size() - 1));
                tempy = std::max(movelist[i][0] - rangey, 0);

                if ((tempx < (int32_t)movemap[0].size()) && (tempy >= 0)) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point32_t = {tempx, tempy};
                            attackmap.push_back(temp_point32_t);
                        }

                        if (mode == "matrix") {
                            attackmap[tempy][tempx] = 1;
                        }
                    }
                }

                tempx = std::max(movelist[i][1] - rangex, 0);
                tempy = std::max(movelist[i][0] - rangey, 0);

                if ((tempx >= 0) && (tempy >= 0)) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point32_t = {tempx, tempy};
                            attackmap.push_back(temp_point32_t);
                        }

                        if (mode == "matrix") {
                            attackmap[tempy][tempx] = 1;
                        }
                    }
                }
            }
        }
    }

    if (mode == "list") {
        // make_uniques
    }

    return (attackmap);
}

template <typename T> extern std::vector<std::vector<T>> movemap(std::vector<std::vector<T>> costmap, uint32_t start[], uint32_t move, std::string mode = "matrix") {
    // Using the map, computes all moveable tiles.
    // outputs either a list of point32_ts, or a map of 1 and zeros.
    struct node {
        T x;
        T y;
        T distance;
    };
    std::vector<std::vector<T>> movemap;

    if (mode == "matrix") {
        movemap = costmap;

        for (int32_t i = 0; i < movemap.size(); i++) {
            std::fill(movemap[i].begin(), movemap[i].end(), 0);
        }
    }

    std::vector<node> open, closed;
    node current, neighbor;
    current.x = start[0];
    current.y = start[1];
    current.distance = 0;
    open.push_back(current);
    int32_t index[2] = {-1, 1};

    while (!open.empty()) {
        current = open.back();
        open.pop_back();
        closed.push_back(current);

        if (mode == "matrix") {
            movemap[current.y][current.x] = 1;
        }

        if (mode == "list") {
            movemap.push_back({current.x, current.y});
        }

        // Two next whiles check the 4 neighbors.
        // (i-j)/2 == 1 when (i+j)/2 == 0 and vice versa.
        for (int32_t i = 0; i < 2; i++) {
            for (int32_t j = 0; j < 2; j++) {
                neighbor.x = std::min(std::max(current.x + ((index[i] + index[j]) / 2), 0), T(costmap[0].size()) - 1);
                neighbor.y = std::min(std::max(current.y + ((index[i] - index[j]) / 2), 0), T(costmap.size()) - 1);
                neighbor.distance = current.distance + costmap[neighbor.y][neighbor.x];

                if ((neighbor.distance <= move) && (costmap[neighbor.y][neighbor.x] > 0)) {
                    bool inclosed = false;

                    for (int32_t k = 0; k < closed.size(); k++) {
                        if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {
                            inclosed = true;

                            if (neighbor.distance < closed[k].distance) {
                                inclosed = false;
                                closed.erase(closed.begin() + k);
                            }

                            break;
                        }
                    }

                    if (!inclosed) {
                        open.push_back(neighbor);
                    }
                }
            }
        }
    }

    return (movemap);
}

#endif /* PATHFINDING_HPP */