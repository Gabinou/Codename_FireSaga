#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <string>
#include <vector>
#include <cmath>

extern int h_manhattan(int start[], int end[]);
extern double h_euclidean(int start[], int end[]);

extern std::vector<std::vector<int>> A_star(std::vector<std::vector<int>> map, int start[], int end[], std::string mode = "matrix");

template <typename T> extern std::vector<std::vector<T>> attackmap(std::vector<std::vector<T>> movemap, short unsigned int start[], short unsigned int move, unsigned char range[2], std::string mode = "matrix") {
    // Using the movemap to computes all attackable tiles.
    // EXCLUDING moveable tiles.
    std::vector<std::vector<T>> attackmap;
    std::vector<std::vector<T>> movelist;
    std::vector<T> temp_point = {0, 0};
    T tempx, tempy;

    movelist = matrix2list(movemap);

    if (mode == "matrix") {
        attackmap = movemap;

        for (int i = 0; i < attackmap.size(); i++) {
            std::fill(attackmap[i].begin(), attackmap[i].end(), 0);
        }
    }

    for (int i = 0; i < movelist.size(); i++) {
        for (int rangex = 0; rangex <= range[1]; rangex++) {
            int miny = std::max(0, range[0] - rangex);
            int maxy = std::max(range[1] - rangex, 0);

            for (int rangey = miny; rangey <= maxy; rangey++) {
                tempx = std::min(movelist[i][1] + rangex, (int)(movemap[0].size() - 1));
                tempy = std::min(movelist[i][0] + rangey, (int)(movemap.size() - 1));

                if ((tempx < (int)movemap[0].size()) && (tempy < (int)movemap.size())) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point = {tempx, tempy};
                            attackmap.push_back(temp_point);
                        }

                        if (mode == "matrix") {
                            attackmap[tempy][tempx] = 1;
                        }
                    }
                }

                tempx = std::max(movelist[i][1] - rangex, 0);
                tempy = std::min(movelist[i][0] + rangey, (int)(movemap.size() - 1));

                if ((tempx >= 0) && (tempy < (int)movemap.size())) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point = {tempx, tempy};
                            attackmap.push_back(temp_point);
                        }

                        if (mode == "matrix") {
                            attackmap[tempy][tempx] = 1;
                        }
                    }
                }

                tempx = std::min(movelist[i][1] + rangex, (int)(movemap[0].size() - 1));
                tempy = std::max(movelist[i][0] - rangey, 0);

                if ((tempx < (int)movemap[0].size()) && (tempy >= 0)) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point = {tempx, tempy};
                            attackmap.push_back(temp_point);
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
                            temp_point = {tempx, tempy};
                            attackmap.push_back(temp_point);
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


template <typename T> extern std::vector<std::vector<T>> movemap(std::vector<std::vector<T>> map, short unsigned int start[], short unsigned int move, std::string mode = "matrix") {
    // Using the map, computes all moveable tiles.
    // outputs either a list of points, or a map of 1 and zeros.
    // Both outputs are 2D vectors.
    struct node {
        T x;
        T y;
        T distance;
    };
    std::vector<std::vector<T>> movemap;

    if (mode == "matrix") {
        movemap = map;

        for (int i = 0; i < movemap.size(); i++) {
            std::fill(movemap[i].begin(), movemap[i].end(), 0);
        }
    }

    std::vector<node> open, closed;
    node current, neighbor;
    current.x = start[0];
    current.y = start[1];
    current.distance = 0;
    open.push_back(current);
    int index[2] = {-1, 1};

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
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                neighbor.x = std::min(std::max(current.x + ((index[i] + index[j]) / 2), 0), T(map[0].size()) - 1);
                neighbor.y = std::min(std::max(current.y + ((index[i] - index[j]) / 2), 0), T(map.size()) - 1);
                neighbor.distance = current.distance + map[neighbor.y][neighbor.x];

                if ((neighbor.distance <= move) && (map[neighbor.y][neighbor.x] > 0)) {
                    bool inclosed = false;

                    for (int k = 0; k < closed.size(); k++) {
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