
#include "pathfinding.h"
#include "stb_ds.h"


// REMOVE MALLOC INSIDE FUNCTIONS
// get rid of stb_ds dependency.

extern int_tile_t * Pathfinding_Map_PushPullable(struct Square_Neighbors direction_block, struct Square_Neighbors pushpullable, size_t row_len, size_t col_len, struct Point start, uint8_t mode_output) {
    SDL_Log("Pathfinding_Map_PushPullable");
    int_tile_t * pushpullablemap = NULL;
    int_tile_t temp_distance;
    struct TilePoint pushpullable_tile;
    int_point_t * block_ptr = (int_point_t *)&direction_block;
    int_point_t * pushpullable_ptr = (int_point_t *)&pushpullable;
    switch (mode_output) {
        case (POINTS_LIST):
            pushpullablemap = DARR_INIT(pushpullablemap, int_tile_t, row_len * col_len * TWO_D);
            break;
        case (POINTS_MATRIX):
            pushpullablemap = calloc(row_len * col_len, sizeof(int_tile_t));
            for (uint8_t row = 0; row < row_len; row++) {
                for (uint8_t col = 0; col < col_len; col++) {
                    pushpullablemap[(row * col_len + col)] = PUSHPULLMAP_BLOCKED;
                }
            }
            break;
    }

    pushpullablemap[start.y * col_len + start.x] = 0;

    for (int_tile_t sq_neighbor = 0; sq_neighbor < SQUARE_NEIGHBOURS; sq_neighbor++) {
        temp_distance = *(block_ptr + sq_neighbor);
        // printf("sq_neighbor %d, temp_distance %d n", sq_neighbor, temp_distance);
        if (*(pushpullable_ptr + sq_neighbor) >= PUSHPULLMAP_MINDIST) {
            for (int_tile_t distance = 1; distance < temp_distance; distance++) {
                pushpullable_tile.x = equation_inbounds((distance * cycle4_pzmz(sq_neighbor)) + start.x, 0, col_len - 1);
                pushpullable_tile.y = equation_inbounds((distance * cycle4_zmzp(sq_neighbor)) + start.y, 0, row_len - 1);
                switch (mode_output) {
                    case POINTS_LIST:
                        DARR_PUT(pushpullablemap, pushpullable_tile.x);
                        DARR_PUT(pushpullablemap, pushpullable_tile.y);
                        break;
                    case POINTS_MATRIX:
                        pushpullablemap[pushpullable_tile.y * col_len + pushpullable_tile.x] = distance;
                        break;
                }
            }
        }
    }
    return (pushpullablemap);
}

int_tile_t * Pathfinding_Map_unitGradient(int_tile_t * in_costmap, size_t row_len, size_t col_len, struct Point * in_targets, size_t unit_num) {
    SDL_Log("Pathfinding_Map_unitGradient");
    int_tile_t * unitgradientmap = calloc(row_len * col_len, sizeof(int_tile_t));

    for (int_tile_t row = 0; row < row_len; row++) {
        for (int_tile_t col = 0; col < col_len; col++) {
            if (in_costmap[row * col_len + col] < PUSHPULLMAP_BLOCKED) {
                unitgradientmap[row * col_len + col] = GRADIENTMAP_BLOCKED;
            } else {
                unitgradientmap[row * col_len + col] = row_len + col_len;
            }
        }
    }

    struct Node * open = DARR_INIT(open, struct Node, row_len * col_len);
    struct Node * closed = DARR_INIT(closed, struct Node, row_len * col_len);

    struct Node current, neighbor;
    for (int_tile_t i = 0; i < unit_num; i++) {
        unitgradientmap[in_targets[i].y * col_len + in_targets[i].x] = GRADIENTMAP_UNIT;
        current.x = in_targets[i].x;
        current.y = in_targets[i].y;
        current.distance = 1;
        DARR_PUT(open, current);
    }

    while (DARR_NUM(open) > 0) {
        current = DARR_POP(open);
        DARR_PUT(closed, current);

        for (int_tile_t sq_neighbor = 0; sq_neighbor < SQUARE_NEIGHBOURS; sq_neighbor++) {
            neighbor.x = equation_inbounds(cycle4_mzpz(sq_neighbor) + current.x, 0, col_len - 1);
            neighbor.y = equation_inbounds(cycle4_zmzp(sq_neighbor) + current.y, 0, row_len - 1);
            neighbor.distance = unitgradientmap[current.y * col_len + current.x] + 1;
            if (in_costmap[neighbor.y * col_len + neighbor.x] >= COSTMAP_MOVEABLEMIN) {
                if (neighbor.distance < unitgradientmap[neighbor.y * col_len + neighbor.x]) {
                    unitgradientmap[neighbor.y * col_len + neighbor.x] = neighbor.distance;
                }
                bool neighbor_inclosed = false;
                for (int32_t k = 0; k < DARR_NUM(closed); k++) {
                    if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {
                        neighbor_inclosed = true;
                        if (neighbor.distance < closed[k].distance) {
                            neighbor_inclosed = false;
                            size_t num = DARR_NUM(closed);
                            size_t size = sizeof(*closed);
                            DARR_DEL(closed, k);
                        }
                        break;
                    }
                }
                if (!neighbor_inclosed) {
                    DARR_PUT(open, neighbor);
                }
            }
        }

    }
    return (unitgradientmap);
}

struct Square_Neighbors Pathfinding_Direction_Pushable(int_tile_t * assailablemap, size_t row_len, size_t col_len, int_tile_t range[2], struct Point target) {
    SDL_Log("Pathfinding_Direction_Pushable");
    struct Square_Neighbors pushable = {0, 0, 0, 0};
    struct Point neighbor;
    for (int_tile_t distance = range[0]; distance <= range[1]; distance++) {
        for (int_tile_t i = 0; i < SQUARE_NEIGHBOURS; i++) {
            neighbor.x = equation_inbounds((target.x + distance * cycle4_pzmz(i)), 0, col_len - 1);
            neighbor.y = equation_inbounds((target.y + distance * cycle4_zpzm(i)), 0, row_len - 1);
            if (assailablemap[neighbor.y * col_len + neighbor.x] >= ASSAILABLE_MOVEABLEMIN) {
                if (neighbor.x > target.x) {
                    pushable.left = true;
                } else if (neighbor.y > target.y) {
                    pushable.top = true;
                } else if (neighbor.x < target.x) {
                    pushable.right = true;
                } else if (neighbor.y < target.y) {
                    pushable.bottom = true;
                }
            }
        }
    }
    return (pushable);
}

struct Square_Neighbors Pathfinding_Direction_Pullable(int_tile_t * assailablemap, size_t row_len, size_t col_len, int_tile_t range[2], struct Point target) {
    SDL_Log("Pathfinding_Direction_Pullable");
    struct Square_Neighbors pullable = {0, 0, 0, 0};
    struct Point neighbor;
    int_point_t * pullable_ptr = (int_point_t *)&pullable;
    SDL_assert(range[0] < range[1]);
    for (int_tile_t distance = range[0]; distance <= range[1]; distance++) {
        for (int_tile_t i = 0; i < SQUARE_NEIGHBOURS; i++) {
            neighbor.x = equation_inbounds((target.x + distance * cycle4_pzmz(i)), 0, col_len - 1);
            neighbor.y = equation_inbounds((target.y + distance * cycle4_zmzp(i)), 0, row_len - 1);
            if ((assailablemap[neighbor.y * col_len + neighbor.x] > 0) && (neighbor.x != 0) && (neighbor.x != (col_len - 1)) && (neighbor.y != 0) && (neighbor.x != (row_len - 1))) {
                *(pullable_ptr + i) = true;
            }
        }
    }

    return (pullable);
}

struct Square_Neighbors Pathfinding_Direction_Block(int_tile_t * costmap_pushpull, size_t row_len, size_t col_len, struct Point start) {
    SDL_Log("Pathfinding_Direction_Block");
    struct Square_Neighbors distance_block = {0, 0, 0, 0};
    int_point_t * distance_ptr = (int_point_t *)&distance_block;
    struct Point neighbor = {0, 0};
    int_tile_t distance = 0;
    while ((distance_block.top == 0) || (distance_block.bottom == 0) || (distance_block.left == 0) || (distance_block.right == 0)) {
        distance++;
        for (int_tile_t i = 0; i < SQUARE_NEIGHBOURS; i++) {
            neighbor.x = equation_inbounds((start.x + cycle4_pzmz(i) * distance), 0, col_len - 1);
            neighbor.y = equation_inbounds((start.y + cycle4_zpzm(i) * distance), 0, row_len - 1);

            if ((costmap_pushpull[neighbor.y * col_len + neighbor.x] == 0) && (*(distance_ptr + i) == 0)) {
                *(distance_ptr + i) = distance;
            }

            if ((neighbor.x == 0) && (distance_block.left == 0)) {
                distance_block.left = start.x;
            } else if ((neighbor.y == 0) && (distance_block.top == 0)) {
                distance_block.top = start.y;
            } else if ((neighbor.x == (col_len - 1)) && (distance_block.right == 0)) {
                distance_block.right = (col_len - 1) - start.x;
            } else if ((neighbor.y == (row_len - 1)) && (distance_block.bottom == 0)) {
                distance_block.bottom = (row_len - 1) - start.y;
            }
        }
    }
    return (distance_block);
}

extern int_tile_t distance_manhattan(struct Point start, struct Point end) {
    int_tile_t distance = abs(start.x - end.x) + abs(start.y - end.y);
    return (distance);
}

extern int_tile_t distance_euclidian(struct Point start, struct Point end) {
    int_tile_t term_x = (start.x - end.x) > INT32_MAX / (start.x - end.x) ? INT32_MAX : (start.x - end.x) * (start.x - end.x);
    int_tile_t term_y = (start.y - end.y) > INT32_MAX / (start.y - end.y) ? INT32_MAX : (start.y - end.y) * (start.y - end.y);

    int_tile_t distance = (int_tile_t)carmack_sqrt_int32_t(term_x) + (int_tile_t)carmack_sqrt_int32_t(term_y) ;
}

bool pathfinding_Point_isReachable(struct Point in_point, int_tile_t * in_movemap, size_t ROW_LEN, size_t COL_LEN) {
    SDL_Log("Pathfinding_Map_Assailable");
    bool out;
    if ((equation_inbounds(in_point.x, 0, COL_LEN)) && (equation_inbounds(in_point.y, 0, ROW_LEN))) {
        out = (in_movemap[in_point.y * COL_LEN + in_point.x] > 0);
    }
    return (out);
}

int_tile_t * Pathfinding_Map_Assailable(int_tile_t * in_movemap, size_t row_len, size_t col_len, struct Point in_target, int_tile_t range[2], uint8_t mode_output) {
    SDL_Log("Pathfinding_Map_Assailable");
    struct Point perimeter_point, delta;
    int_tile_t * assailablemap = NULL;

    switch (mode_output) {
        case (POINTS_LIST):
            assailablemap = DARR_INIT(assailablemap, int_tile_t, row_len * col_len * TWO_D);
            break;
        case (POINTS_MATRIX):
            assailablemap = calloc(row_len * col_len, sizeof(int_tile_t));
            for (uint8_t row = 0; row < row_len; row++) {
                for (uint8_t col = 0; col < col_len; col++) {
                    assailablemap[(row * col_len + col)] = ASSAILABLE_BLOCKED;
                }
            }
            break;
    }

    for (int_tile_t i_range = range[0]; i_range <= range[1]; i_range++) {
        for (int_tile_t sq_neighbor = 0; sq_neighbor < (i_range * SQUARE_NEIGHBOURS); sq_neighbor++) {
            delta.x = equation_inbounds(i_range * cycle4_mzpz(sq_neighbor) + (sq_neighbor / SQUARE_NEIGHBOURS) * cycle4_pmmp(sq_neighbor), -in_target.x, col_len - in_target.x);
            delta.y = equation_inbounds(i_range * cycle4_zmzp(sq_neighbor) + (sq_neighbor / SQUARE_NEIGHBOURS) * cycle4_ppmm(sq_neighbor), -in_target.y, row_len - in_target.y);
            perimeter_point.x = in_target.x + delta.x;
            perimeter_point.y = in_target.y + delta.y;
            if (in_movemap[perimeter_point.y * col_len + perimeter_point.x] >= MOVEMAP_MOVEABLEMIN) {
                switch (mode_output) {
                    case POINTS_LIST:
                        if (!Point_inList_2D(assailablemap, DARR_NUM(assailablemap) / TWO_D, perimeter_point.x, perimeter_point.y)) {
                            DARR_PUT(assailablemap, perimeter_point.x);
                            DARR_PUT(assailablemap, perimeter_point.y);
                        }
                        break;
                    case POINTS_MATRIX:
                        assailablemap[perimeter_point.y * col_len + perimeter_point.x] = i_range;
                        break;
                }
            }
        }
    }
    return (assailablemap);
}

int_tile_t * Pathfinding_Map_Attack(int_tile_t * move_matrix, size_t row_len, size_t col_len, int_tile_t move, int_tile_t range[2], uint8_t mode_output, uint8_t mode_movetile) {
    SDL_Log("Pathfinding_Map_Attack");
    // Using the move_matrix to compute all attackable tiles.
    int_tile_t * attackmap = NULL, *temp_row = NULL, *move_list = NULL, * toadd = NULL;
    int_tile_t subrangey_min, subrangey_max;
    struct Point temp_point;
    move_list = matrix2list_int16_t(move_matrix, row_len, col_len);
    size_t list_len = DARR_LEN(move_list) / TWO_D;

    switch (mode_output) {
        case (POINTS_LIST):
            attackmap = DARR_INIT(attackmap, int_tile_t, row_len * col_len * TWO_D);
            break;
        case (POINTS_MATRIX):
            attackmap = calloc(row_len * col_len, sizeof(int_tile_t));
            for (uint8_t row = 0; row < row_len; row++) {
                for (uint8_t col = 0; col < col_len; col++) {
                    attackmap[(row * col_len + col)] = ATTACKMAP_BLOCKED;
                }
            }
            break;
    }

    bool add_point;
    switch (mode_movetile) {
        case MOVETILE_INCLUDE:
            add_point = true;
            break;
        default:
            add_point = true;
            break;
    }
    for (int_tile_t i = 0; i < list_len; i++) {
        // SDL_Log("list_len %d %d %d", i, move_list[i * TWO_D + 0], move_list[i * TWO_D + 1]);
        for (int_tile_t rangex = 0; rangex <= range[1]; rangex++) {
            subrangey_min = (rangex > range[0]) ? 0 : (range[0] - rangex);
            subrangey_max = (rangex > range[1]) ? 0 : (range[1] - rangex);
            for (int_tile_t rangey = subrangey_min; rangey <= subrangey_max; rangey++) {
                for (int_tile_t sq_neighbor = 0; sq_neighbor < SQUARE_NEIGHBOURS; sq_neighbor++) {
                    temp_point.x = equation_inbounds(move_list[i * TWO_D + 0] + cycle4_pmmp(sq_neighbor) * rangex, 0, col_len - 1);
                    temp_point.y = equation_inbounds(move_list[i * TWO_D + 1] + cycle4_ppmm(sq_neighbor) * rangey, 0, row_len - 1);
                    switch (mode_movetile) {
                        case MOVETILE_EXCLUDE:
                            add_point = (move_matrix[temp_point.y * col_len + temp_point.x] == MOVEMAP_BLOCKED);
                            break;
                    }
                    if (add_point) {
                        switch (mode_output) {
                            case POINTS_LIST:
                                if (!Point_inList_2D(attackmap, DARR_NUM(attackmap) / TWO_D, temp_point.x, temp_point.y)) {
                                    DARR_PUT(attackmap, temp_point.x)
                                    DARR_PUT(attackmap, temp_point.y)
                                }
                                break;
                            case POINTS_MATRIX:
                                attackmap[temp_point.y * col_len + temp_point.x] = ATTACKMAP_MOVEABLEMIN;
                                break;
                        }
                    }
                }
            }
        }
    }
    return (attackmap);
}

int_tile_t * Pathfinding_Map_Move_Hex(int_tile_t * cost_matrix, size_t depth_len, size_t col_len, struct HexPoint start, int_tile_t move, uint8_t mode_output) {
    SDL_Log("Pathfinding_Map_Move_Hex");
    int_tile_t * move_matrix = NULL, * temp_row = NULL;

    switch (mode_output) {
        case (POINTS_LIST):
            move_matrix = DARR_INIT(move_matrix, int_tile_t, depth_len * col_len * TWO_D);
            break;
        case (POINTS_MATRIX):
            move_matrix = calloc(depth_len * col_len, sizeof(int_tile_t));
            for (uint8_t depth = 0; depth < depth_len; depth++) {
                for (uint8_t col = 0; col < col_len; col++) {
                    move_matrix[(depth * col_len + col)] = MOVEMAP_BLOCKED;
                }
            }
            break;
    }

    struct HexNode * open = DARR_INIT(open, struct HexNode, depth_len * col_len);
    struct HexNode * closed = DARR_INIT(closed, struct HexNode, depth_len * col_len);

    struct HexNode current = {start.x, start.y, start.z, 0}, neighbor;
    DARR_PUT(open, current);
    bool found;
    while (DARR_NUM(open) > 0) {
        current = DARR_POP(open);
        DARR_PUT(closed, current);

        switch (mode_output) {
            case POINTS_MATRIX:
                if ((move_matrix[current.z * col_len + current.x] == 0) || (move_matrix[current.z * col_len + current.x] > (current.distance + 1))) {
                    move_matrix[current.z * col_len + current.x] = current.distance + 1;
                }
                break;
            case POINTS_LIST:
                found = Point_inList_2D(move_matrix, DARR_NUM(move_matrix) / TWO_D, current.x, current.z);
                if (!found) {
                    DARR_PUT(move_matrix, current.x);
                    DARR_PUT(move_matrix, current.z);
                }
                break;
        }
        for (int_tile_t hex_neighbor = 0; hex_neighbor < HEXAGON_NEIGHBOURS; hex_neighbor++) {
            neighbor.x = equation_inbounds(current.x + cycle6_mppmzz(hex_neighbor), 0, col_len - 1);
            neighbor.z = equation_inbounds(current.z + cycle6_pmzzmp(hex_neighbor), 0, depth_len - 1);
            if (cost_matrix[current.z * col_len + current.x] >= 0) {
                neighbor.distance = current.distance + cost_matrix[current.z * col_len + current.x];
                if ((neighbor.distance <= move) && (cost_matrix[neighbor.z * col_len + neighbor.x] >= COSTMAP_MOVEABLEMIN)) {
                    bool neighbor_inclosed = false;
                    for (int32_t k = 0; k < DARR_NUM(closed); k++) {
                        if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)  && (neighbor.z == closed[k].z)) {
                            neighbor_inclosed = true;
                            if (neighbor.distance < closed[k].distance) {
                                neighbor_inclosed = false;
                                size_t num = DARR_NUM(closed);
                                size_t size = sizeof(*closed);
                                DARR_DEL(closed, k);
                            }
                            break;
                        }
                    }
                    if (!neighbor_inclosed) {
                        DARR_PUT(open, neighbor);
                    }
                }
            }
        }
    }
    return (move_matrix);
}

int_tile_t * Pathfinding_Map_Move(int_tile_t * cost_matrix, size_t row_len, size_t col_len, struct Point start, int_tile_t move, uint8_t mode_output) {
    SDL_Log("Pathfinding_Map_Move");
    int_tile_t * move_matrix = NULL, * temp_row = NULL;
    switch (mode_output) {
        case (POINTS_LIST):
            move_matrix = DARR_INIT(move_matrix, int_tile_t, row_len * col_len * TWO_D);
            break;
        case (POINTS_MATRIX):
            move_matrix = calloc(row_len * col_len, sizeof(int_tile_t));
            for (uint8_t row = 0; row < row_len; row++) {
                for (uint8_t col = 0; col < col_len; col++) {
                    move_matrix[(row * col_len + col)] = MOVEMAP_BLOCKED;
                }
            }
            break;
    }

    struct Node * open = DARR_INIT(open, struct Node, row_len * col_len * 2);
    struct Node * closed = DARR_INIT(closed, struct Node, row_len * col_len * 2);

    struct Node current = {start.x, start.y, 0}, neighbor;
    open[DARR_NUM(open)++] = current;

    bool found, neighbor_inclosed;
    while (DARR_NUM(open) > 0) {
        current = DARR_POP(open);
        DARR_PUT(closed, current);

        switch (mode_output) {
            case POINTS_MATRIX:
                if ((move_matrix[current.y * col_len + current.x] == MOVEMAP_BLOCKED) || (move_matrix[current.y * col_len + current.x] > (current.distance + 1))) {
                    move_matrix[current.y * col_len + current.x] = current.distance + 1;
                }
                break;
            case POINTS_LIST:
                found = Point_inList_2D(move_matrix, DARR_NUM(move_matrix) / TWO_D, current.x, current.y);
                if (!found) {
                    DARR_PUT(move_matrix, current.x);
                    DARR_PUT(move_matrix, current.y);
                }
                break;
        }
        for (int_tile_t i = 0; i < SQUARE_NEIGHBOURS; i++) {
            neighbor.x = equation_inbounds(current.x + cycle4_mzpz(i), 0, col_len - 1);
            neighbor.y = equation_inbounds(current.y + cycle4_zmzp(i), 0, row_len - 1);
            neighbor.distance = current.distance + cost_matrix[neighbor.y * col_len + neighbor.x];
            if ((neighbor.distance <= move) && (cost_matrix[neighbor.y * col_len + neighbor.x] >= COSTMAP_MOVEABLEMIN)) {
                neighbor_inclosed = false;
                for (int32_t k = 0; k < DARR_NUM(closed); k++) {
                    if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {
                        neighbor_inclosed = true;
                        if (neighbor.distance < closed[k].distance) {
                            neighbor_inclosed = false;
                            size_t num = DARR_NUM(closed);
                            size_t size = sizeof(*closed);
                            DARR_DEL(closed, k);
                        }
                        break;
                    }
                }
                if (!neighbor_inclosed) {
                    DARR_PUT(open, neighbor);
                }
            }
        }
    }
    return (move_matrix);
}

int_tile_t * Pathfinding_Map_Sight(int_tile_t * block_matrix, size_t row_len, size_t col_len, struct Point start, int_tile_t sight, uint8_t mode_output) {
    SDL_Log("Pathfinding_Map_Sight");
    int_tile_t * sightmap = NULL;
    struct Point perimeter_point = {0, 0}, delta = {0, 0}, interpolated = {0, 0};
    bool visible;

    switch (mode_output) {
        case (POINTS_LIST):
            sightmap = DARR_INIT(sightmap, int_tile_t, row_len * col_len * TWO_D);
            break;
        case (POINTS_MATRIX):
            sightmap = calloc(row_len * col_len, sizeof(int_tile_t));
            for (uint8_t row = 0; row < row_len; row++) {
                for (uint8_t col = 0; col < col_len; col++) {
                    sightmap[(row * col_len + col)] = SIGHTMAP_NOT_VISIBLE;
                }
            }
            break;
    }

    sightmap[start.y * col_len + start.x] = SIGHTMAP_OBSERVER;

    for (int_tile_t distance = 1; distance <= sight; distance++) {
        for (int_tile_t sq_neighbor = 0; sq_neighbor < (distance * SQUARE_NEIGHBOURS); sq_neighbor++) {
            delta.x = equation_inbounds(distance * cycle4_mzpz(sq_neighbor) + (sq_neighbor / SQUARE_NEIGHBOURS) * cycle4_pmmp(sq_neighbor), -start.x, col_len - start.x);
            delta.y = equation_inbounds(distance * cycle4_zmzp(sq_neighbor) + (sq_neighbor / SQUARE_NEIGHBOURS) * cycle4_ppmm(sq_neighbor), -start.y, row_len - start.y);
            perimeter_point.x = start.x + delta.x;
            perimeter_point.y = start.y + delta.y;
            visible = true;
            for (int32_t interp_dist = 1; interp_dist < distance; interp_dist++) {
                interpolated.x = start.x + (delta.x == 0 ? 0 : (int32_t)lround(interp_dist * delta.x * (1.0f / distance)));
                interpolated.y = start.y + (delta.y == 0 ? 0 : (int32_t)lround(interp_dist * delta.y * (1.0f / distance)));
                if ((interpolated.x != start.x) || (interpolated.y != start.y)) {
                    if (block_matrix[interpolated.y * col_len + interpolated.x] >= BLOCKMAP_MIN) {
                        visible = false;
                        break;
                    }
                }
            }
            if (visible) {
                switch (mode_output) {
                    case (POINTS_MATRIX):
                        switch (block_matrix[perimeter_point.y * col_len + perimeter_point.x]) {
                            case BLOCKMAP_BLOCKED:
                                sightmap[perimeter_point.y * col_len + perimeter_point.x] = SIGHTMAP_VISIBLE;
                                break;
                            default:
                                sightmap[perimeter_point.y * col_len + perimeter_point.x] = SIGHTMAP_WALL;
                                break;
                        }
                        break;
                }
            }
        }
    }
    return (sightmap);
}

int_tile_t * Pathfinding_Map_Sight_Hex(int_tile_t * block_matrix, size_t depth_len, size_t col_len, struct HexPoint start, int_tile_t sight, uint8_t mode_output) {
    SDL_Log("Pathfinding_Map_Sight_Hex");
    int_tile_t * sightmap = NULL;
    struct HexPoint perimeter_point = {0, 0, 0}, delta = {0, 0, 0}, interpolated = {0, 0, 0};
    bool visible;

    switch (mode_output) {
        case (POINTS_LIST):
            sightmap = DARR_INIT(sightmap, int_tile_t, depth_len * col_len * TWO_D);
            break;
        case (POINTS_MATRIX):
            sightmap = calloc(depth_len * col_len, sizeof(int_tile_t));
            for (uint8_t depth = 0; depth < depth_len; depth++) {
                for (uint8_t col = 0; col < col_len; col++) {
                    sightmap[(depth * col_len + col)] = SIGHTMAP_NOT_VISIBLE;
                }
            }
            break;
    }
    sightmap[start.z * col_len + start.x] = SIGHTMAP_OBSERVER;

    for (int_tile_t distance = 1; distance <= sight; distance++) {
        for (int_tile_t perimeter_tile = 0; perimeter_tile < (distance * HEXAGON_NEIGHBOURS); perimeter_tile++) { // iterates perimeter tiles at distance
            delta.x = equation_inbounds(distance * cycle6_mppmzz(perimeter_tile) + perimeter_tile / HEXAGON_NEIGHBOURS * cycle6_pmzzmp(perimeter_tile), -start.x, col_len - start.x);
            delta.z = equation_inbounds(distance * cycle6_pmzzmp(perimeter_tile) + perimeter_tile / HEXAGON_NEIGHBOURS * cycle6_zzmppm(perimeter_tile), -start.z, depth_len - start.z);
            perimeter_point.x = start.x + delta.x;
            perimeter_point.z = start.z + delta.z;
            visible = true;
            for (int32_t interp_dist = 1; interp_dist < distance; interp_dist++) {
                interpolated.x = start.x + (delta.x == 0 ? 0 : (int32_t)lround(interp_dist * delta.x * (1.0f / distance)));
                interpolated.z = start.z + (delta.z == 0 ? 0 : (int32_t)lround(interp_dist * delta.z * (1.0f / distance)));
                if ((interpolated.x != start.x) || (interpolated.z != start.z)) {
                    if (block_matrix[interpolated.z * col_len + interpolated.x] >= BLOCKMAP_MIN) {
                        visible = false;
                        break;
                    }
                }
            }
            if (visible) {
                switch (block_matrix[perimeter_point.z * col_len + perimeter_point.x]) {
                    case BLOCKMAP_BLOCKED:
                        sightmap[perimeter_point.z * col_len + perimeter_point.x] = SIGHTMAP_VISIBLE;
                        break;
                    default:
                        sightmap[perimeter_point.z * col_len + perimeter_point.x] = SIGHTMAP_WALL;
                        break;
                }
            }
        }
    }
    return (sightmap);
}

int_tile_t * Pathfinding_Map_Path(int_tile_t * move_matrix, size_t row_len, size_t col_len, struct Point start, struct Point end, uint8_t mode_path) {
    SDL_Log("Pathfinding_Map_Path");
    int_tile_t * path_position = DARR_INIT(path_position, int_tile_t, row_len * col_len * TWO_D);
    int_tile_t * out = DARR_INIT(out, int_tile_t, row_len * col_len * TWO_D);

    struct Point current = end, neighbor, next;
    int_tile_t current_cost;

    if ((move_matrix[start.y * col_len + start.x] >= MOVEMAP_MOVEABLEMIN) || (move_matrix[end.y * col_len + end.x] >= MOVEMAP_MOVEABLEMIN)) {
        while ((current.x != start.x) || (current.y != start.y)) {
            DARR_PUT(path_position, current.x);
            DARR_PUT(path_position, current.y);
            current_cost = INT16_MAX;
            for (int_tile_t sq_neighbor = 0; sq_neighbor < SQUARE_NEIGHBOURS; sq_neighbor++) {
                neighbor.x = equation_inbounds(cycle4_mzpz(sq_neighbor) + current.x, 0, col_len - 1);
                neighbor.y = equation_inbounds(cycle4_zmzp(sq_neighbor) + current.y, 0, row_len - 1);
                if (move_matrix[neighbor.y * col_len + neighbor.x] >= MOVEMAP_MOVEABLEMIN) {
                    if (current_cost > move_matrix[neighbor.y * col_len + neighbor.x]) {
                        current_cost = move_matrix[neighbor.y * col_len + neighbor.x];
                        next = neighbor;
                    }
                }
            }
            current = next;
        }
    } else {
        SDL_Log("Pathfinding_Map_Path invalid start or end point");
        exit(ERROR_PathfindingInvalidStartEnd);
    }
    DARR_PUT(path_position, current.x);
    DARR_PUT(path_position, current.y);
    switch (mode_path) {
        case PATH_STEP:
            for (int_tile_t i = ((DARR_NUM(path_position) / TWO_D) - 1); i > 0 ; i--) {
                DARR_PUT(out, (path_position[(i - 1) * TWO_D + 0] - path_position[i * TWO_D + 0]));
                DARR_PUT(out, (path_position[(i - 1) * TWO_D + 1] - path_position[i * TWO_D + 1]));
            }
            break;
        case PATH_POSITION:
            out = path_position;
            break;
    }
    return (out);
}

int_tile_t * Pathfinding_Path_step2position(int_tile_t * step_list, size_t list_len, struct Point start) {
    SDL_Log("Pathfinding_Path_step2position");
    int_tile_t * path_position = DARR_INIT(path_position, int_tile_t, ((list_len + 1) * 2));
    DARR_PUT(path_position, start.x);
    DARR_PUT(path_position, start.y);

    for (int_tile_t i = 0; i < list_len; i++) {
        DARR_PUT(path_position, (path_position[i * TWO_D + 0] + step_list[i * TWO_D + 0]));
        DARR_PUT(path_position, (path_position[i * TWO_D + 1] + step_list[i * TWO_D + 1]));
    }

    return (path_position);
}
