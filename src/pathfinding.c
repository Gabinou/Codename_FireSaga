
#include "pathfinding.h"

i32 *Taxicab_Circle_List(i32 *darr_list, i32 *matrix, i32 x, i32 y,
                         size_t row_len, size_t col_len, struct Range *range) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // Taxicabs can't move diagonal so manhattan distance: abs(x1-x2) + abs(y1-ys2)
    // Manhattan (distance) used to trace 'circles' on square tilemap
    // Returns: List points at distance [range_min, range_max] dist from [x, y]
    matrix = Taxicab_Circle(matrix, x, y, row_len, col_len, range);
    darr_list = linalg_matrix2list_noM_int32_t(matrix, darr_list, row_len, col_len);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (darr_list);
}

i32 *Taxicab_Circle(i32 *matrix, i32 x, i32 y, size_t row_len, size_t col_len,
                    struct Range *range) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 subrangey_min, subrangey_max;
    for (i32 rangex = 0; rangex <= range->max; rangex++) {
        subrangey_min = (rangex > range->min) ? 0 : (range->min - rangex);
        subrangey_max = (rangex > range->max) ? 0 : (range->max - rangex);
        for (i32 rangey = subrangey_min; rangey <= subrangey_max; rangey++) {
            for (i32 sq_neighbor = 0; sq_neighbor < SQUARE_NEIGHBOURS; sq_neighbor++) {
                i32 tempx = int_inbounds(x + q_cycle4_pmmp(sq_neighbor) * rangex, 0, col_len - 1);
                i32 tempy = int_inbounds(y + q_cycle4_ppmm(sq_neighbor) * rangey, 0, row_len - 1);
                matrix[tempx * col_len + tempy] = 1;
            }
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (matrix);
}


/* --- Pushing and pulling --- */
/* TODO: Convert from nmath to SOTA */

i32 *pathfinding_Map_PushPullto_noM(i32 *pushpulltomap, struct SquareNeighbors  direction_block,
                                    struct SquareNeighbors pushpullto, size_t row_len, size_t col_len, struct Point start) {
    i32 temp_distance;
    struct Point pushpullto_tile;
    i32 *block_ptr         = (i32 *)&direction_block;
    i32 *pushpullto_ptr    = (i32 *)&pushpullto;
    for (size_t i = 0; i < row_len; i++) {
        pushpulltomap[i] = PUSHPULLMAP_BLOCKED;

        pushpulltomap[start.y * col_len + start.x] = 0;
        for (i32  sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {
            temp_distance = *(block_ptr + sq_neighbor);
            if (*(pushpullto_ptr + sq_neighbor) >= NMATH_PUSHPULLMAP_MINDIST) {
                for (i32  distance = 1; distance < temp_distance; distance++) {
                    pushpullto_tile.x = int_inbounds((distance * q_cycle4_pzmz(sq_neighbor)) + start.x, 0, col_len - 1);
                    pushpullto_tile.y = int_inbounds((distance * q_cycle4_zmzp(sq_neighbor)) + start.y, 0, row_len - 1);
                    pushpulltomap[pushpullto_tile.y * col_len + pushpullto_tile.x] = distance;
                }
            }
        }
        return (pushpulltomap);
    }
}


struct SquareNeighbors  pathfinding_Direction_Pushto(i32   *attackfrommap, size_t row_len,
                                                     size_t col_len, int8_t range[2], struct Point target) {
    struct SquareNeighbors  Pushto = {0, 0, 0, 0};
    struct Point neighbor;
    for (i32 distance = range[0]; distance <= range[1]; distance++) {
        for (i32  i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
            neighbor.x = int_inbounds((target.x + distance * q_cycle4_pzmz(i)), 0, col_len - 1);
            neighbor.y = int_inbounds((target.y + distance * q_cycle4_zpzm(i)), 0, row_len - 1);
            if (attackfrommap[neighbor.y * col_len + neighbor.x] >= NMATH_ATTACKFROM_MOVEABLEMIN) {
                if (neighbor.x > target.x) {
                    Pushto.left = true;
                } else if (neighbor.y > target.y) {
                    Pushto.top = true;
                } else if (neighbor.x < target.x) {
                    Pushto.right = true;
                } else if (neighbor.y < target.y) {
                    Pushto.bottom = true;
                }
            }
        }
    }
    return (Pushto);
}

i32 *pathfinding_Map_PushPullto(struct SquareNeighbors  direction_block,
                                struct SquareNeighbors  pushpullto, size_t row_len, size_t col_len, struct Point start,
                                u8 mode_output) {
    i32 *pushpulltomap = NULL;
    i32 temp_distance;
    struct Point pushpullto_tile;
    i32 *block_ptr = (i32 *)&direction_block;
    i32 *pushpullto_ptr = (i32 *)&pushpullto;
    switch (mode_output) {
        case (NMATH_POINTS_MODE_LIST):
            pushpulltomap = DARR_INIT(pushpulltomap, i32, row_len * col_len * NMATH_TWO_D);
            break;
        case (NMATH_POINTS_MODE_MATRIX):
            pushpulltomap = calloc(row_len * col_len, sizeof(*pushpulltomap));
            for (size_t row = 0; row < row_len; row++) {
                for (size_t col = 0; col < col_len; col++) {
                    pushpulltomap[(row * col_len + col)] = NMATH_PUSHPULLMAP_BLOCKED;
                }
            }
            break;
    }
    pushpulltomap[start.y * col_len + start.x] = 0;
    for (i32  sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {
        temp_distance = *(block_ptr + sq_neighbor);
        if (*(pushpullto_ptr + sq_neighbor) >= NMATH_PUSHPULLMAP_MINDIST) {
            for (i32  distance = 1; distance < temp_distance; distance++) {
                pushpullto_tile.x = int_inbounds((distance * q_cycle4_pzmz(sq_neighbor)) + start.x, 0, col_len - 1);
                pushpullto_tile.y = int_inbounds((distance * q_cycle4_zmzp(sq_neighbor)) + start.y, 0, row_len - 1);
                switch (mode_output) {
                    case NMATH_POINTS_MODE_LIST:
                        DARR_PUT(pushpulltomap, pushpullto_tile.x);
                        DARR_PUT(pushpulltomap, pushpullto_tile.y);
                        break;
                    case NMATH_POINTS_MODE_MATRIX:
                        pushpulltomap[pushpullto_tile.y * col_len + pushpullto_tile.x] = distance;
                        break;
                }
            }
        }
    }
    return (pushpulltomap);
}



struct SquareNeighbors  pathfinding_Direction_Block(i32   *costmap_pushpull, size_t row_len,
                                                    size_t col_len, struct Point start) {
    struct SquareNeighbors  distance_block = {0, 0, 0, 0};
    i32 *distance_ptr = (i32 *)&distance_block;
    struct Point neighbor = {0, 0};
    i32  distance = 0;
    while ((distance_block.top == 0) || (distance_block.bottom == 0) || (distance_block.left == 0)
           || (distance_block.right == 0)) {
        distance++;
        for (i32  i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
            neighbor.x = int_inbounds((start.x + q_cycle4_pzmz(i) * distance), 0, col_len - 1);
            neighbor.y = int_inbounds((start.y + q_cycle4_zpzm(i) * distance), 0, row_len - 1);
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

/* --- AStar --- */
i32 *Pathfinding_CameFrom_List(i32 *path, i32 *came_from, size_t col_len,
                               struct Point start, struct Point end) {
    struct Point current = end;
    struct Point move;
    DARR_NUM(path) = 0;
    for (size_t i = 0; i < SOTA_ITERATIONS_LIMIT; i++) {
        DARR_PUT(path, current.x);
        DARR_PUT(path, current.y);
        if ((current.x == start.x) && (current.y == start.y))
            break;
        move = Ternary_Moved(came_from[current.y * col_len + current.x]);
        current.x -= move.x;
        current.y -= move.y;
    }
    return (path);
}

i32 Pathfinding_Manhattan(struct Point start, struct Point end) {
    /* Does not include endpoints */
    i32 distance = labs(start.x - end.x) + labs(start.y - end.y);
    return (distance);
}

i32 *Pathfinding_Astar(i32 *path_list, i32 *costmap, size_t row_len,
                       size_t col_len, struct Point start, struct Point end) {
    /* Assumes square grid, path_list is a DARR */
    /* [1]: http://www.redblobgames.com/pathfinding/a-star/introduction.html */
    /* Checks */
    assert((start.x != end.x) || (start.y != end.y));
    assert(costmap[start.y * col_len + start.x] >= MOVEMAP_MOVEABLEMIN);
    assert(costmap[end.y   * col_len + end.x]   >= MOVEMAP_MOVEABLEMIN);

    /* Resetting path_list */
    DARR_NUM(path_list) = 0;

    /* Alloc variables */
    i32 *cost       = calloc(row_len * col_len, sizeof(*cost));
    i32 *came_from  = calloc(row_len * col_len, sizeof(*came_from));
    struct Nodeq current = {.x = start.x, .y = start.y, .cost = 0};
    struct Nodeq neighbor;

    /* Frontier points queue, by priority. */
    /* Lowest priority (movcost + distance) is top priority, at low index. */
    struct Nodeq *frontier_queue = DARR_INIT(frontier_queue, struct Nodeq, row_len * col_len);
    DARR_PUT(frontier_queue, current);

    /* Run algorithm until no points in frontier */
    while (DARR_NUM(frontier_queue) > 0) {
        current = DARR_POP(frontier_queue);

        /* End loop when end point is reached */
        if ((current.x == end.x) && (current.y == end.y))
            break;

        /* Visit all square neighbors */
        for (size_t sq_neighbor = 0; sq_neighbor < SQUARE_NEIGHBOURS; sq_neighbor++) {
            /* Get next neighbor movement cost */
            neighbor.x = int_inbounds(q_cycle4_mzpz(sq_neighbor) + current.x, 0, col_len - 1);
            neighbor.y = int_inbounds(q_cycle4_zmzp(sq_neighbor) + current.y, 0, row_len - 1);
            int current_n = neighbor.y * col_len + neighbor.x;
            neighbor.cost = current.cost + costmap[current_n];

            /* Compute conditions for adding neighbor to frontier */
            /* Skip neighbor if: blocked */
            if (costmap[current_n] < MOVEMAP_MOVEABLEMIN)
                continue;

            /* Skip neighbor if:  already visited AND higher cost */
            bool higher_cost  = (neighbor.cost >= cost[current_n]);
            bool visited      = (cost[   current_n] > 0);
            if (visited && higher_cost)
                continue;

            /* distance is heuristic for closeness to goal */
            size_t distance = linalg_distance_manhattan_int32_t(end.x, end.y, neighbor.x, neighbor.y);
            cost[current_n] = neighbor.cost;

            /* Djikstra algo only has cost in this step */
            neighbor.priority = neighbor.cost + distance;

            /* Find index to insert neighbor into priority queue. */
            size_t index = DARR_NUM(frontier_queue);
            while ((index > 0) && (neighbor.priority > frontier_queue[index - 1].priority))
                index--;

            DARR_INSERT(frontier_queue, neighbor, index);
            struct Point move = {neighbor.x - current.x, neighbor.y - current.y};

            /* Update came_from, to build better path */
            came_from[current_n] = Ternary_Direction(move);
        }
    }
    path_list = Pathfinding_CameFrom_List(path_list, came_from, col_len, start, end);
    free(cost);
    free(came_from);
    DARR_FREE(frontier_queue);
    return (path_list);
}

i32 *Pathfinding_Moveto(i32 *cost_matrix, size_t row_len, size_t col_len,
                        struct Point start, i32 move, int mode_output) {

    /* -- Setup output move_matrix -- */
    i32 *move_matrix = NULL;
    switch (mode_output) {
        case (POINTS_MODE_LIST):
            move_matrix = DARR_INIT(move_matrix, i32, row_len * col_len * TWO_D);
            break;
        case (POINTS_MODE_MATRIX):
            move_matrix = calloc(row_len * col_len, sizeof(*move_matrix));
            for (size_t row = 0; row < row_len; row++) {
                for (size_t col = 0; col < col_len; col++) {
                    move_matrix[(row * col_len + col)] = MOVEMAP_BLOCKED;
                }
            }
            break;
    }

    /* -- Setup variables -- */
    size_t init_size = row_len * col_len / 4;
    struct Node *open     = DARR_INIT(open,   struct Node, init_size);
    struct Node *closed   = DARR_INIT(closed, struct Node, init_size);
    struct Node current   = {start.x, start.y, 0}, neighbor;
    DARR_PUT(open, current);
    bool found;

    /* -- Loop over open nodes -- */
    while (DARR_NUM(open) > 0) {
        /* -- Get current open node from open list -- */
        current = DARR_POP(open);
        DARR_PUT(closed, current);

        /* -- Compute cost to current tile -- */
        i32 current_i   = current.y * col_len + current.x;
        i32 move_i      = move_matrix[current_i];
        if (mode_output == POINTS_MODE_MATRIX) {
            if ((move_i == MOVEMAP_BLOCKED) || (move_i > (current.distance + 1)))
                move_matrix[current_i] = current.distance + 1;
        } else if (mode_output == POINTS_MODE_LIST) {
            size_t pnum = DARR_NUM(move_matrix) / TWO_D;
            if (!linalg_list_isIn_2D_int32_t(move_matrix, pnum, current.x, current.y)) {
                DARR_PUT(move_matrix, current.x);
                DARR_PUT(move_matrix, current.y);
            }
        }

        /* -- Move to four square neighbor tiles -- */
        Pathfinding_Moveto_Neighbors(open, closed, current, cost_matrix,
                                     row_len, col_len, move);
    }
    DARR_FREE(open);
    DARR_FREE(closed);
    return (move_matrix);
}

void Pathfinding_Moveto_noM(i32 *move_matrix, i32 *cost_matrix, size_t row_len,
                            size_t col_len, struct Point start, i32 move) {
    /* -- Wipe move_matrix -- */
    for (size_t i = 0; i < row_len * col_len; i++)
        move_matrix[i] = MOVEMAP_BLOCKED;

    /* -- Setup variables -- */
    size_t init_size    = row_len * col_len * 2;
    struct Node *open   = DARR_INIT(open,   struct Node, init_size);
    struct Node *closed = DARR_INIT(closed, struct Node, init_size);
    struct Node current = {start.x, start.y, 0};
    DARR_PUT(open, current);
    bool neighbor_inclosed;

    /* -- Loop over open nodes -- */
    while (DARR_NUM(open) > 0) {
        /* -- Get current open node from open list -- */
        current = DARR_POP(open);
        DARR_PUT(closed, current);

        /* -- Compute cost to current tile -- */
        i32 current_i   = current.y * col_len + current.x;
        i32 move_i      = move_matrix[current_i];
        if (( move_i == MOVEMAP_BLOCKED) || (move_i > (current.distance + 1)))
            move_matrix[current_i] = current.distance + 1;

        /* -- Move to four square neighbor tiles -- */
        Pathfinding_Moveto_Neighbors(open, closed, current, cost_matrix,
                                     row_len, col_len, move);
    }

    DARR_FREE(open);
    DARR_FREE(closed);
}

void Pathfinding_Moveto_Neighbors(struct Node *open, struct Node *closed,
                                  struct Node current, i32 *cost_matrix,
                                  size_t row_len, size_t col_len, i32 move) {
    struct Node neighbor;

    /* -- Move to four square neighbor tiles -- */
    for (size_t i = 0; i < SQUARE_NEIGHBOURS; i++) {
        /* - Get square neighbor - */
        neighbor.x = int_inbounds(current.x + q_cycle4_mzpz(i), 0, col_len - 1);
        neighbor.y = int_inbounds(current.y + q_cycle4_zmzp(i), 0, row_len - 1);

        /* - Get total cost to neighbor - */
        int current_n       = neighbor.y * col_len + neighbor.x;
        neighbor.distance   = current.distance + cost_matrix[current_n];

        /* - Skip if neighbor is out of reach - */
        if ((neighbor.distance > move) || (cost_matrix[current_n] == 0))
            continue;

        /* - Find if neighbor was already visited -> in closed - */
        int neighbor_inclosed = -1;
        for (i32 k = 0; k < DARR_NUM(closed); k++) {
            if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {
                neighbor_inclosed = k;
                break;
            }
        }

        /* - Add neighbor to open list, if not visited before - */
        if (neighbor_inclosed < 0) {
            DARR_PUT(open, neighbor);
            continue;
        }

        /* - Add neighbor to open list, if closer than before - */
        if (neighbor.distance < closed[neighbor_inclosed].distance) {
            DARR_DEL(closed, neighbor_inclosed);
            DARR_PUT(open,   neighbor);
        }
    }
}

/* -- Attackto -- */
void Pathfinding_Attackto_noM(i32 *attackmap, i32 *move_matrix,
                              size_t row_len, size_t col_len,
                              u8 range[2], i32 mode_movetile) {
    /* -- Wipe attackmap -- */
    for (u8 i = 0; i < row_len * col_len; i++)
        attackmap[i] = ATTACKMAP_BLOCKED;

    /* -- Setup variables -- */
    i32 *move_list  = linalg_matrix2list_int32_t(move_matrix, row_len, col_len);
    size_t list_len = DARR_NUM(move_list) / TWO_D;

    /* -- For every point in movemap -- */
    for (i32 i = 0; i < list_len; i++) {
        i32 x = move_list[i * TWO_D + 0];
        i32 y = move_list[i * TWO_D + 1];

        Pathfinding_Attackto_Neighbors(x, y, attackmap, move_matrix, row_len,
                                       col_len, range, mode_movetile);
    }
    DARR_FREE(list_len);
}

i32 *Pathfinding_Attackto(i32 *move_matrix, size_t row_len, size_t col_len,
                          u8 range[2], i32 mode_movetile) {
    /* -- Setup output attackmap -- */
    i32 *attackmap = calloc(row_len * col_len, sizeof(*attackmap));
    for (u8 i = 0; i < row_len * col_len; i++)
        attackmap[i] = NMATH_ATTACKMAP_BLOCKED;

    /* -- Setup variables -- */
    i32 *move_list  = linalg_matrix2list_int32_t(move_matrix, row_len, col_len);
    size_t list_len = DARR_NUM(move_list) / NMATH_TWO_D;
    // bool add_point  = (mode_movetile != MOVETILE_EXCLUDE);

    /* -- For every point in movemap -- */
    for (i32 i = 0; i < list_len; i++) {
        i32 x = move_list[i * TWO_D + 0];
        i32 y = move_list[i * TWO_D + 1];

        Pathfinding_Attackto_Neighbors(x, y, attackmap, move_matrix, row_len,
                                       col_len, range, mode_movetile);
    }
    free(move_list);
}

void Pathfinding_Attackto_Neighbors(i32 x, i32 y, i32 *attackmap, i32 *move_matrix,
                                    size_t row_len, size_t col_len,
                                    u8 range[2], i32 mode_movetile) {
    struct Point point;
    bool add_point  = (mode_movetile != MOVETILE_EXCLUDE);

    /* -- Setup output attackmap -- */
    /* -- Iterate over possible ranges in x -- */
    for (i32 rangex = 0; rangex <= range[1]; rangex++) {
        i32 subrangey_min = (rangex > range[0]) ? 0 : (range[0] - rangex);
        i32 subrangey_max = (rangex > range[1]) ? 0 : (range[1] - rangex);
        /* -- Iterate over possible ranges in y, knowing x range -- */
        for (i32 rangey = subrangey_min; rangey <= subrangey_max; rangey++) {
            /* -- Iterate over range 4 combinations: x+y+, x+y-, x-y+, x-y- */
            for (i32 sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {
                point.x = int_inbounds(x + q_cycle4_pmmp(sq_neighbor) * rangex, 0, col_len - 1);
                point.y = int_inbounds(y + q_cycle4_ppmm(sq_neighbor) * rangey, 0, row_len - 1);

                if (mode_movetile == NMATH_MOVETILE_EXCLUDE)
                    add_point = (move_matrix[point.y * col_len + point.x] == MOVEMAP_BLOCKED);

                /* Skip if not adding point to attackmap */
                if (!add_point)
                    continue;

                attackmap[point.y * col_len + point.x] = NMATH_ATTACKMAP_MOVEABLEMIN;
            }
        }
    }
}

/* -- Unit Gradient -- */
i32 *pathfinding_Map_unitGradient_noM(i32 *unitgradientmap, i32 *in_costmap, size_t row_len,
                                      size_t col_len, struct Point *in_targets, size_t unit_num) {
    for (i32  col = 0; col < col_len; col++) {
        for (i32  row = 0; row < row_len; row++) {
            if (in_costmap[row * col_len + col] < NMATH_PUSHPULLMAP_BLOCKED) {
                unitgradientmap[row * col_len + col] = NMATH_GRADIENTMAP_BLOCKED;
            } else {
                unitgradientmap[row * col_len + col] = row_len + col_len;
            }
        }
    }
    struct Node *open = DARR_INIT(open, struct Node, row_len * col_len);
    struct Node *closed = DARR_INIT(closed, struct Node, row_len * col_len);
    struct Node current, neighbor;
    for (i32  i = 0; i < unit_num; i++) {
        unitgradientmap[in_targets[i].y * col_len + in_targets[i].x] = NMATH_GRADIENTMAP_UNIT;
        current.x = in_targets[i].x;
        current.y = in_targets[i].y;
        current.distance = 1;
        DARR_PUT(open, current);
    }
    while (DARR_NUM(open) > 0) {
        current = DARR_POP(open);
        DARR_PUT(closed, current);
        for (i32  sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {
            neighbor.x = int_inbounds(q_cycle4_mzpz(sq_neighbor) + current.x, 0, col_len - 1);
            neighbor.y = int_inbounds(q_cycle4_zmzp(sq_neighbor) + current.y, 0, row_len - 1);
            neighbor.distance = unitgradientmap[current.y * col_len + current.x] + 1;
            if (in_costmap[neighbor.y * col_len + neighbor.x] >= NMATH_COSTMAP_MOVEABLEMIN) {
                if (neighbor.distance < unitgradientmap[neighbor.y * col_len + neighbor.x]) {
                    unitgradientmap[neighbor.y * col_len + neighbor.x] = neighbor.distance;
                }
                bool neighbor_inclosed = false;
                for (int32_t k = 0; k < DARR_NUM(closed); k++) {
                    if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {
                        neighbor_inclosed = true;
                        if (neighbor.distance < closed[k].distance) {
                            neighbor_inclosed = false;
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



i32 *pathfinding_Map_unitGradient(i32 *in_costmap, size_t row_len, size_t col_len,
                                  struct Point *in_targets, size_t unit_num) {
    i32 *unitgradientmap = calloc(row_len * col_len, sizeof(i32));
    for (i32  col = 0; col < col_len; col++) {
        for (i32  row = 0; row < row_len; row++) {
            if (in_costmap[row * col_len + col] < NMATH_PUSHPULLMAP_BLOCKED) {
                unitgradientmap[row * col_len + col] = NMATH_GRADIENTMAP_BLOCKED;
            } else {
                unitgradientmap[row * col_len + col] = row_len + col_len;
            }
        }
    }
    struct Node *open = DARR_INIT(open, struct Node, row_len * col_len);
    struct Node *closed = DARR_INIT(closed, struct Node, row_len * col_len);
    struct Node current, neighbor;
    for (i32  i = 0; i < unit_num; i++) {
        unitgradientmap[in_targets[i].y * col_len + in_targets[i].x] = NMATH_GRADIENTMAP_UNIT;
        current.x = in_targets[i].x;
        current.y = in_targets[i].y;
        current.distance = 1;
        DARR_PUT(open, current);
    }
    while (DARR_NUM(open) > 0) {
        current = DARR_POP(open);
        DARR_PUT(closed, current);
        for (i32  sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {
            neighbor.x = int_inbounds(q_cycle4_mzpz(sq_neighbor) + current.x, 0, col_len - 1);
            neighbor.y = int_inbounds(q_cycle4_zmzp(sq_neighbor) + current.y, 0, row_len - 1);
            neighbor.distance = unitgradientmap[current.y * col_len + current.x] + 1;
            if (in_costmap[neighbor.y * col_len + neighbor.x] >= NMATH_COSTMAP_MOVEABLEMIN) {
                if (neighbor.distance < unitgradientmap[neighbor.y * col_len + neighbor.x]) {
                    unitgradientmap[neighbor.y * col_len + neighbor.x] = neighbor.distance;
                }
                bool neighbor_inclosed = false;
                for (int32_t k = 0; k < DARR_NUM(closed); k++) {
                    if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {
                        neighbor_inclosed = true;
                        if (neighbor.distance < closed[k].distance) {
                            neighbor_inclosed = false;
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



/* -- Visible -- */
i32 *pathfinding_Map_Visible_noM(i32 *sightmap, i32 *block_matrix, size_t row_len, size_t col_len,
                                 struct Point start, i32 sight) {
    struct Point perimeter_nmath_point = {0, 0}, delta = {0, 0}, interpolated = {0, 0};
    bool visible;
    for (u8 row = 0; row < row_len; row++) {
        for (u8 col = 0; col < col_len; col++) {
            sightmap[(row * col_len + col)] = NMATH_SIGHTMAP_BLOCKED;
        }
    }
    sightmap[start.y * col_len + start.x] = NMATH_SIGHTMAP_OBSERVER;
    for (i32  distance = 1; distance <= sight; distance++) {
        for (i32  sq_neighbor = 0; sq_neighbor < (distance * NMATH_SQUARE_NEIGHBOURS); sq_neighbor++) {
            delta.x = int_inbounds(distance * q_cycle4_mzpz(sq_neighbor) + (sq_neighbor /
                                   NMATH_SQUARE_NEIGHBOURS) * q_cycle4_pmmp(sq_neighbor), -start.x, col_len - start.x);
            delta.y = int_inbounds(distance * q_cycle4_zmzp(sq_neighbor) + (sq_neighbor /
                                   NMATH_SQUARE_NEIGHBOURS) * q_cycle4_ppmm(sq_neighbor), -start.y, row_len - start.y);
            perimeter_nmath_point.x = start.x + delta.x;
            perimeter_nmath_point.y = start.y + delta.y;
            visible = true;
            for (int32_t interp_dist = 1; interp_dist < distance; interp_dist++) {
                interpolated.x = start.x + (delta.x == 0 ? 0 : (int32_t)lround(interp_dist * delta.x *
                                            (1.0f / distance)));
                interpolated.y = start.y + (delta.y == 0 ? 0 : (int32_t)lround(interp_dist * delta.y *
                                            (1.0f / distance)));
                if ((interpolated.x != start.x) || (interpolated.y != start.y)) {
                    if (block_matrix[interpolated.y * col_len + interpolated.x] >= NMATH_BLOCKMAP_MIN) {
                        visible = false;
                        break;
                    }
                }
            }
            if (visible) {
                switch (block_matrix[perimeter_nmath_point.y * col_len + perimeter_nmath_point.x]) {
                    case NMATH_BLOCKMAP_BLOCKED:
                        sightmap[perimeter_nmath_point.y * col_len + perimeter_nmath_point.x] = NMATH_SIGHTMAP_VISIBLE;
                        break;
                    default:
                        sightmap[perimeter_nmath_point.y * col_len + perimeter_nmath_point.x] = NMATH_SIGHTMAP_WALL;
                        break;
                }
            }
        }
    }
    return (sightmap);
}



i32 *pathfinding_Map_Visible(i32   *block_matrix, size_t row_len, size_t col_len,
                             struct Point start, i32 sight, u8 mode_output) {
    i32   *sightmap = NULL;
    struct Point perimeter_nmath_point = {0, 0}, delta = {0, 0}, interpolated = {0, 0};
    bool visible;
    switch (mode_output) {
        case (NMATH_POINTS_MODE_LIST):
            sightmap = DARR_INIT(sightmap, i32, row_len * col_len * NMATH_TWO_D);
            break;
        case (NMATH_POINTS_MODE_MATRIX):
            sightmap = calloc(row_len * col_len, sizeof(i32));
            for (u8 row = 0; row < row_len; row++) {
                for (u8 col = 0; col < col_len; col++) {
                    sightmap[(row * col_len + col)] = NMATH_SIGHTMAP_BLOCKED;
                }
            }
            break;
    }
    sightmap[start.y * col_len + start.x] = NMATH_SIGHTMAP_OBSERVER;
    for (i32  distance = 1; distance <= sight; distance++) {
        for (i32  sq_neighbor = 0; sq_neighbor < (distance * NMATH_SQUARE_NEIGHBOURS); sq_neighbor++) {
            delta.x = int_inbounds(distance * q_cycle4_mzpz(sq_neighbor) + (sq_neighbor /
                                   NMATH_SQUARE_NEIGHBOURS) * q_cycle4_pmmp(sq_neighbor), -start.x, col_len - start.x);
            delta.y = int_inbounds(distance * q_cycle4_zmzp(sq_neighbor) + (sq_neighbor /
                                   NMATH_SQUARE_NEIGHBOURS) * q_cycle4_ppmm(sq_neighbor), -start.y, row_len - start.y);
            perimeter_nmath_point.x = start.x + delta.x;
            perimeter_nmath_point.y = start.y + delta.y;
            visible = true;
            for (int32_t interp_dist = 1; interp_dist < distance; interp_dist++) {
                interpolated.x = start.x + (delta.x == 0 ? 0 : (int32_t)lround(interp_dist * delta.x *
                                            (1.0f / distance)));
                interpolated.y = start.y + (delta.y == 0 ? 0 : (int32_t)lround(interp_dist * delta.y *
                                            (1.0f / distance)));
                if ((interpolated.x != start.x) || (interpolated.y != start.y)) {
                    if (block_matrix[interpolated.y * col_len + interpolated.x] >= NMATH_BLOCKMAP_MIN) {
                        visible = false;
                        break;
                    }
                }
            }
            if (visible) {
                switch (mode_output) {
                    case (NMATH_POINTS_MODE_MATRIX):
                        switch (block_matrix[perimeter_nmath_point.y * col_len + perimeter_nmath_point.x]) {
                            case NMATH_BLOCKMAP_BLOCKED:
                                sightmap[perimeter_nmath_point.y * col_len + perimeter_nmath_point.x] = NMATH_SIGHTMAP_VISIBLE;
                                break;
                            default:
                                sightmap[perimeter_nmath_point.y * col_len + perimeter_nmath_point.x] = NMATH_SIGHTMAP_WALL;
                                break;
                        }
                        break;
                }
            }
        }
    }
    return (sightmap);
}


