
#include "pathfinding.h"

/* --- Manhattan distance --- */
i32 Pathfinding_Manhattan(struct Point start, struct Point end) {
    /* Does not include endpoints */
    i32 distance = labs(start.x - end.x) + labs(start.y - end.y);
    return (distance);
}

i32 _Pathfinding_Manhattan(i32 x_0, i32 y_0, i32 x_1, i32 y_1) {
    /* Does not include endpoints */
    i32 distance = labs(x_0 - x_1) + labs(y_0 - y_1);
    return (distance);
}

/* --- Taxicab Geometry --- */
i32 *Taxicab_Circle_List(i32 *darr_list, i32 *matrix, i32 x, i32 y,
                         size_t row_len, size_t col_len, struct Range *range) {
    // Taxicabs can't move diagonal so manhattan distance: abs(x1-x2) + abs(y1-ys2)
    // Manhattan (distance) used to trace 'circles' on square tilemap
    // Returns: List points at distance [range_min, range_max] dist from [x, y]
    matrix    = Taxicab_Circle(matrix, x, y, row_len, col_len, range);
    darr_list = matrix2list_noM(matrix, darr_list, row_len, col_len);
    return (darr_list);
}

i32 *Taxicab_Circle(i32 *matrix, i32 x, i32 y, size_t row_len, size_t col_len,
                    struct Range *range) {
    i32 subrangey_min, subrangey_max;
    for (i32 rangex = 0; rangex <= range->max; rangex++) {
        subrangey_min = (rangex > range->min) ? 0 : (range->min - rangex);
        subrangey_max = (rangex > range->max) ? 0 : (range->max - rangex);
        for (i32 rangey = subrangey_min; rangey <= subrangey_max; rangey++) {
            for (i32 n = 0; n < SQUARE_NEIGHBOURS; n++) {
                i32 tempx = int_inbounds(x + q_cycle4_pmmp(n) * rangex, 0, col_len - 1);
                i32 tempy = int_inbounds(y + q_cycle4_ppmm(n) * rangey, 0, row_len - 1);
                matrix[tempx * col_len + tempy] = 1;
            }
        }
    }
    return (matrix);
}

/* --- Pushing and pulling --- */
// TODO: refactor pushpull stuff to be BETTER
i32 *Pathfinding_PushPullto_noM(i32 *pushpulltomap,
                                struct SquareNeighbours direction_block,
                                struct SquareNeighbours pushpullto,
                                size_t row_len, size_t col_len,
                                struct Point start) {
    i32 temp_distance;
    struct Point pushpullto_tile;
    i32 *block_ptr         = (i32 *)&direction_block;
    i32 *pushpullto_ptr    = (i32 *)&pushpullto;
    for (size_t i = 0; i < row_len; i++) {
        pushpulltomap[i] = PUSHPULLMAP_BLOCKED;

        pushpulltomap[start.y * col_len + start.x] = 0;
        for (i32 n = 0; n < NMATH_SQUARE_NEIGHBOURS; n++) {
            temp_distance = *(block_ptr + n);
            if (*(pushpullto_ptr + n) >= NMATH_PUSHPULLMAP_MINDIST) {
                for (i32  distance = 1; distance < temp_distance; distance++) {
                    pushpullto_tile.x = int_inbounds((distance * q_cycle4_pzmz(n)) + start.x, 0, col_len - 1);
                    pushpullto_tile.y = int_inbounds((distance * q_cycle4_zmzp(n)) + start.y, 0, row_len - 1);
                    pushpulltomap[pushpullto_tile.y * col_len + pushpullto_tile.x] = distance;
                }
            }
        }
    }

    return (pushpulltomap);
}


struct SquareNeighbours pathfinding_Direction_Pushto(i32 *attackfrommap, size_t row_len,
                                                     size_t col_len, u8 range[2], struct Point target) {
    struct SquareNeighbours Pushto = {0, 0, 0, 0};
    struct Point neighbour;
    for (i32 distance = range[0]; distance <= range[1]; distance++) {
        for (i32  i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
            neighbour.x = int_inbounds((target.x + distance * q_cycle4_pzmz(i)), 0, col_len - 1);
            neighbour.y = int_inbounds((target.y + distance * q_cycle4_zpzm(i)), 0, row_len - 1);
            if (attackfrommap[neighbour.y * col_len + neighbour.x] >= NMATH_ATTACKFROM_MOVEABLEMIN) {
                if (neighbour.x > target.x) {
                    Pushto.left = true;
                } else if (neighbour.y > target.y) {
                    Pushto.top = true;
                } else if (neighbour.x < target.x) {
                    Pushto.right = true;
                } else if (neighbour.y < target.y) {
                    Pushto.bottom = true;
                }
            }
        }
    }

    return (Pushto);
}

i32 *Pathfinding_PushPullto(struct SquareNeighbours direction_block,
                            struct SquareNeighbours pushpullto,
                            size_t row_len, size_t col_len,
                            struct Point start, u8 mode_output) {
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
    for (i32  n = 0; n < NMATH_SQUARE_NEIGHBOURS; n++) {
        temp_distance = *(block_ptr + n);
        if (*(pushpullto_ptr + n) >= NMATH_PUSHPULLMAP_MINDIST) {
            for (i32  distance = 1; distance < temp_distance; distance++) {
                pushpullto_tile.x = int_inbounds((distance * q_cycle4_pzmz(n)) + start.x, 0,
                                                 col_len - 1);
                pushpullto_tile.y = int_inbounds((distance * q_cycle4_zmzp(n)) + start.y, 0,
                                                 row_len - 1);
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

struct SquareNeighbours pathfinding_Direction_Block(i32 *traversemap_pushpull, size_t row_len,
                                                    size_t col_len, struct Point start) {
    struct SquareNeighbours  distance_block = {0, 0, 0, 0};
    i32 *distance_ptr = (i32 *)&distance_block;
    struct Point neighbour = {0, 0};
    i32  distance = 0;
    while ((distance_block.top == 0) || (distance_block.bottom == 0) || (distance_block.left == 0)
           || (distance_block.right == 0)) {
        distance++;
        for (i32  i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
            neighbour.x = int_inbounds((start.x + q_cycle4_pzmz(i) * distance), 0, col_len - 1);
            neighbour.y = int_inbounds((start.y + q_cycle4_zpzm(i) * distance), 0, row_len - 1);
            if ((traversemap_pushpull[neighbour.y * col_len + neighbour.x] == 0)
                && (*(distance_ptr + i) == 0)) {
                *(distance_ptr + i) = distance;
            }
            if ((neighbour.x == 0) && (distance_block.left == 0)) {
                distance_block.left = start.x;
            } else if ((neighbour.y == 0) && (distance_block.top == 0)) {
                distance_block.top = start.y;
            } else if ((neighbour.x == (col_len - 1)) && (distance_block.right == 0)) {
                distance_block.right = (col_len - 1) - start.x;
            } else if ((neighbour.y == (row_len - 1)) && (distance_block.bottom == 0)) {
                distance_block.bottom = (row_len - 1) - start.y;
            }
        }
    }

    return (distance_block);
}

/* --- AStar --- */
/* path_list -> end at 0, start at num */
i32 *Pathfinding_PathList_Forward(i32 *path, i32 *came_from, size_t col_len,
                                  struct Point start, struct Point end) {
    struct Point current = end;
    struct Point move;
    DARR_NUM(path) = 0;
    for (size_t i = 0; i < SOTA_ITERATIONS_LIMIT; i++) {
        DARR_INSERT(path, current.y, 0);
        DARR_INSERT(path, current.x, 0);
        if ((current.x == start.x) && (current.y == start.y))
            break;
        move = Ternary_Moved(came_from[current.y * col_len + current.x]);
        current.x -= move.x;
        current.y -= move.y;
    }
    return (path);
}
/* path_list -> start at 0, end at num */
i32 *Pathfinding_PathList_Backward(i32 *path, i32 *came_from, size_t col_len,
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

/* Astar pathfinding + occupiable tiles, moving n tiles accross path to target */
i32 *Pathfinding_Astar_plus(i32 *path_list, i32 *traversemap, tnecs_entity *occupymap,
                            size_t row_len, size_t col_len, int move,
                            struct Point start, struct Point end, b32 forward) {
    /* Assumes square grid, path_list is a DARR */
    /* [1]: http://www.redblobgames.com/pathfinding/a-star/introduction.html */
    /* -- Checks -- */
    SDL_assert((start.x != end.x) || (start.y != end.y));
    SDL_assert(traversemap[start.y * col_len + start.x] >= MOVEMAP_MOVEABLEMIN);
    SDL_assert(traversemap[end.y   * col_len + end.x]   >= MOVEMAP_MOVEABLEMIN);

    /* Alloc variables */
    i32 *cost       = calloc(row_len * col_len, sizeof(*cost));
    i32 *came_from  = calloc(row_len * col_len, sizeof(*came_from));
    struct Nodeq current = {.x = start.x, .y = start.y, .cost = 0};
    struct Nodeq neighbour;
    struct Point closest = start; /* In case target is blocked*/

    /* Frontier points queue, by priority. */
    /* Lowest priority (movcost + distance) is top priority, at low index. */
    struct Nodeq *frontier_queue = DARR_INIT(frontier_queue, struct Nodeq, row_len * col_len);
    DARR_PUT(frontier_queue, current);

    /* -- Run algorithm until no points in frontier -- */
    while (DARR_NUM(frontier_queue) > 0) {
        current = DARR_POP(frontier_queue);
        /* - End loop when end point is reached - */
        if ((current.x == end.x) && (current.y == end.y))
            break;

        /* -- Find closest tile to end in case we can't reach -- */
        // Note: Use Manhnattan distance to find closest tile,
        //       cause tiles close to end might be blocked
        int current_dist = _Pathfinding_Manhattan(end.x, end.y, current.x, current.y);
        int closest_dist = _Pathfinding_Manhattan(end.x, end.y, closest.x, closest.y);
        if (current_dist < closest_dist) {
            closest.x = current.x;
            closest.y = current.y;
        }

        /* -- Visit all square neighbours -- */
        for (size_t n = 0; n < SQUARE_NEIGHBOURS; n++) {
            /* Get next neighbour movement cost */
            neighbour.x     = int_inbounds(q_cycle4_mzpz(n) + current.x, 0, col_len - 1);
            neighbour.y     = int_inbounds(q_cycle4_zmzp(n) + current.y, 0, row_len - 1);
            int current_n   = neighbour.y * col_len + neighbour.x;
            neighbour.cost  = current.cost + traversemap[current_n];

            /* Compute conditions for adding neighbour to frontier */
            /* Skip neighbour if: blocked */
            if (traversemap[current_n] < MOVEMAP_MOVEABLEMIN)
                continue;

            /* Skip neighbour if: cost greater than move */
            if (neighbour.cost > move)
                continue;

            /* Skip neighbour if: tile is occupied */
            // short-circuit on NULL occupymap  
            if ((occupymap != NULL) && (occupymap[current_n] != TNECS_NULL))
                continue;

            /* Skip neighbour if: already visited AND higher cost */
            bool higher_cost  = (neighbour.cost >= cost[current_n]);
            bool visited      = (cost[current_n] > 0);
            if (visited && higher_cost)
                continue;

            /* distance is heuristic for closeness to goal */
            size_t distance = _Pathfinding_Manhattan(end.x, end.y, neighbour.x, neighbour.y);
            cost[current_n] = neighbour.cost;

            /* Djikstra algo only has cost in this step */
            neighbour.priority = neighbour.cost + distance;

            /* Find index to insert neighbour into priority queue. */
            size_t index = DARR_NUM(frontier_queue);
            while ((index > 0) && (neighbour.priority > frontier_queue[index - 1].priority))
                index--;

            DARR_INSERT(frontier_queue, neighbour, index);
            struct Point move = {neighbour.x - current.x, neighbour.y - current.y};

            /* Update came_from, to build better path */
            came_from[current_n] = Ternary_Direction(move);
        }
    }
    PathList_f forward_f    = &Pathfinding_PathList_Forward;
    PathList_f backward_f   = &Pathfinding_PathList_Backward;
    PathList_f pathlist_f   = forward ? forward_f : backward_f;

    /* Resetting path_list */
    DARR_NUM(path_list) = 0;
    if ((current.x == end.x) && (current.y == end.y))
        /* End reached, get path to it*/
        path_list = pathlist_f(path_list, came_from, col_len, start, end);
    else
        /* End NOT reached, get path to closest tile */
        path_list = pathlist_f(path_list, came_from, col_len, start, closest);

    SDL_free(cost);
    SDL_free(came_from);
    DARR_FREE(frontier_queue);

    return (path_list);
}

i32 *Pathfinding_Astar(i32 *path_list, i32 *traversemap, size_t row_len, size_t col_len,
                       struct Point start, struct Point end, b32 forward) {
    path_list = Pathfinding_Astar_plus(path_list, traversemap, NULL,
                                       row_len, col_len, INT_MAX,
                                       start, end, forward);
    return (path_list);
}

i32 *Pathfinding_Moveto(i32 *cost_matrix, size_t row_len, size_t col_len,
                        struct Point start, i32 move) {
    /* -- Setup output move_matrix -- */
    i32 *move_matrix = calloc(row_len * col_len, sizeof(*move_matrix));
    Pathfinding_Moveto_noM(move_matrix, cost_matrix, row_len, col_len, start, move);

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
    bool neighbour_inclosed;

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

        /* -- Move to four square neighbour tiles -- */
        Pathfinding_Moveto_Neighbours(open, closed, current, cost_matrix,
                                      row_len, col_len, move);
    }

    DARR_FREE(open);
    DARR_FREE(closed);

}

void Pathfinding_Moveto_Neighbours(struct Node *open, struct Node *closed,
                                   struct Node current, i32 *cost_matrix,
                                   size_t row_len, size_t col_len, i32 move) {
    struct Node neighbour;

    /* -- Move to four square neighbour tiles -- */
    for (size_t i = 0; i < SQUARE_NEIGHBOURS; i++) {
        /* - Get square neighbour - */
        neighbour.x = int_inbounds(current.x + q_cycle4_mzpz(i), 0, col_len - 1);
        neighbour.y = int_inbounds(current.y + q_cycle4_zmzp(i), 0, row_len - 1);

        /* - Get total cost to neighbour - */
        int current_n       = neighbour.y * col_len + neighbour.x;
        neighbour.distance  = current.distance + cost_matrix[current_n];

        /* - Skip if neighbour is out of reach - */
        if ((neighbour.distance > move) || (cost_matrix[current_n] == 0))
            continue;

        /* - Decide to add neighbor to open list or not - */
        Pathfinding_Neighbour(open, closed, neighbour);
    }

}

void Pathfinding_Neighbour(struct Node *open, struct Node *closed, struct Node neighbour) {
    /* - Find if neighbour was already visited -> in closed - */
    int neighbour_inclosed = -1;
    for (i32 k = 0; k < DARR_NUM(closed); k++) {
        if ((neighbour.x == closed[k].x) && (neighbour.y == closed[k].y)) {
            neighbour_inclosed = k;
            break;
        }
    }

    /* - Add neighbour to open list, if not visited before - */
    if (neighbour_inclosed < 0) {
        DARR_PUT(open, neighbour);
        return;
    }

    /* - Add neighbour to open list, if closer than before - */
    if (neighbour.distance < closed[neighbour_inclosed].distance) {
        DARR_DEL(closed, neighbour_inclosed);
        DARR_PUT(open,   neighbour);
    }

}

/* -- Attackto -- */
void Pathfinding_Attackto_noM(i32 *attackmap, i32 *move_matrix,
                              size_t row_len, size_t col_len,
                              u8 range[2], i32 mode_movetile) {
    /* -- Wipe attackmap -- */
    for (i32 i = 0; i < row_len * col_len; i++)
        attackmap[i] = ATTACKMAP_BLOCKED;

    /* -- Setup variables -- */
    i32 *move_list  = matrix2list(move_matrix, row_len, col_len);
    size_t list_len = DARR_NUM(move_list) / TWO_D;

    /* -- For every point in movemap -- */
    for (i32 i = 0; i < list_len; i++) {
        i32 x = move_list[i * TWO_D + 0];
        i32 y = move_list[i * TWO_D + 1];

        Pathfinding_Attackto_Neighbours(x, y, attackmap, move_matrix, row_len,
                                        col_len, range, mode_movetile);
    }
    DARR_FREE(move_list);

}

i32 *Pathfinding_Attackto(i32 *move_matrix, size_t row_len, size_t col_len,
                          u8 range[2], i32 mode_movetile) {
    /* -- Setup output attackmap -- */
    i32 *attackmap = calloc(row_len * col_len, sizeof(*attackmap));
    Pathfinding_Attackto_noM(attackmap, move_matrix, row_len, col_len, range, mode_movetile);

    return (attackmap);
}

void Pathfinding_Attackto_Neighbours(i32 x, i32 y, i32 *attackmap, i32 *move_matrix,
                                     size_t row_len, size_t col_len,
                                     u8 range[2], i32 mode_movetile) {
    /* -- Setup variables -- */
    struct Point point;
    bool add_point  = (mode_movetile != MOVETILE_EXCLUDE);

    /* -- Iterate over possible ranges in x -- */
    for (i32 rangex = 0; rangex <= range[1]; rangex++) {
        i32 subrangey_min = (rangex > range[0]) ? 0 : (range[0] - rangex);
        i32 subrangey_max = (rangex > range[1]) ? 0 : (range[1] - rangex);
        /* -- Iterate over possible ranges in y, knowing x range -- */
        for (i32 rangey = subrangey_min; rangey <= subrangey_max; rangey++) {
            /* -- Iterate over range 4 combinations: x+y+, x+y-, x-y+, x-y- */
            for (i32 n = 0; n < NMATH_SQUARE_NEIGHBOURS; n++) {
                point.x = int_inbounds(x + q_cycle4_pmmp(n) * rangex, 0, col_len - 1);
                point.y = int_inbounds(y + q_cycle4_ppmm(n) * rangey, 0, row_len - 1);

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
void Pathfinding_unitGradient_noM(i32 *gradmap, i32 *traversemap,
                                  size_t row_len, size_t col_len,
                                  struct Point *targets, size_t unit_num) {
    /* -- Wipe gradmap -- */
    for (i32 i = 0; i < col_len * row_len; i++) {
        if (traversemap[i] < NMATH_PUSHPULLMAP_BLOCKED)
            gradmap[i] = NMATH_GRADIENTMAP_BLOCKED;
        else
            gradmap[i] = row_len + col_len;
    }

    /* -- Setup nodes -- */
    struct Node *open   = DARR_INIT(open,   struct Node, row_len * col_len);
    struct Node *closed = DARR_INIT(closed, struct Node, row_len * col_len);
    struct Node current, neighbour;

    /* -- Setup units on gradmap + open nodes -- */
    for (i32 i = 0; i < unit_num; i++) {
        gradmap[targets[i].y * col_len + targets[i].x] = NMATH_GRADIENTMAP_UNIT;
        current.x = targets[i].x;
        current.y = targets[i].y;
        current.distance = 1;
        DARR_PUT(open, current);
    }

    /* -- Loop over open nodes -- */
    while (DARR_NUM(open) > 0) {
        /* -- Get current open node from open list -- */
        current = DARR_POP(open);
        DARR_PUT(closed, current);

        /* -- Visit all square neighbours -- */
        for (i32 n = 0; n < NMATH_SQUARE_NEIGHBOURS; n++) {

            neighbour.x = int_inbounds(q_cycle4_mzpz(n) + current.x, 0, col_len - 1);
            neighbour.y = int_inbounds(q_cycle4_zmzp(n) + current.y, 0, row_len - 1);
            neighbour.distance = gradmap[current.y * col_len + current.x] + 1;
            int current_n = neighbour.y * col_len + neighbour.x;

            /* Skip neighbour if: blocked */
            if (traversemap[current_n] < TRAVERSEMAP_MIN)
                continue;

            /* Skip neighbour if:  already visited AND higher cost */
            if (neighbour.distance >= gradmap[current_n])
                continue;

            /* Update gradmap if closer than when visited previously */
            gradmap[current_n] = neighbour.distance;

            Pathfinding_Neighbour(open, closed, neighbour);
        }
    }

}

i32 *Pathfinding_unitGradient(i32 *traversemap, size_t row_len, size_t col_len,
                              struct Point *targets, size_t unit_num) {
    i32 *gradmap = calloc(row_len * col_len, sizeof(i32));
    Pathfinding_unitGradient_noM(gradmap, traversemap, row_len, col_len, targets, unit_num);

    return (gradmap);
}

/* -- Visible -- */
bool Pathfinding_Tile_Visible(i32 *sightmap, i32 *blockmap, struct Point start,
                              struct Point delta, size_t col_len) {
    /* -- Between start and delta -- */
    i32 distance = abs(delta.x) + abs(delta.y);

    bool visible = true;
    for (i32 d = 1; d <= distance; d++) {
        i32 dist_x = (i32)lround(d * delta.x * (1.0f / distance));
        i32 dist_y = (i32)lround(d * delta.y * (1.0f / distance));

        struct Point interpolated;
        interpolated.x = start.x + dist_x;
        interpolated.y = start.y + dist_y;

        i32 interp_i = interpolated.y * col_len + interpolated.x;
        if (blockmap[interp_i] == BLOCKMAP_BLOCKED) {
            /* -- Perimeter tile is blocked -- */
            sightmap[interp_i] = SIGHTMAP_BLOCKED;
            visible = false;
            break;
        }
    }

    return (visible);
}

void Pathfinding_Visible_noM(i32 *sightmap, i32 *blockmap,
                             size_t row_len, size_t col_len,
                             struct Point start, i32 sight) {
    /* -- Wipe sightmap -- */

    for (int i = 0; i < row_len * col_len; i++)
        sightmap[i] = SIGHTMAP_NOT_VISIBLE;

    /* -- Setup variables -- */
    struct Point perimeter_point = {0}, delta = {0};
    sightmap[start.y * col_len + start.x] = SIGHTMAP_OBSERVER;

    /* -- Loop over all visible tiles -- */
    for (i32 distance = 1; distance <= sight; distance++) {
        for (i32 n = 0; n < (distance * NMATH_SQUARE_NEIGHBOURS); n++) {
            /* -- Find perimeter tile at edge of visible distance -- */
            //  Example components:
            //  _1 --------->    e.g. component 1 goes to max,
            //     |distance|
            //  _2      ^
            //  _2      |--      e.g. component 2 walks back to perimeter
            i32 x_1  = distance * q_cycle4_mzpz(n);
            i32 y_1  = distance * q_cycle4_zmzp(n);
            i32 x_2  = (n / NMATH_SQUARE_NEIGHBOURS) * q_cycle4_pmmp(n);
            i32 y_2  = (n / NMATH_SQUARE_NEIGHBOURS) * q_cycle4_ppmm(n);

            delta.x = int_inbounds(x_1 + x_2, -start.x, col_len - start.x);
            delta.y = int_inbounds(y_1 + y_2, -start.y, row_len - start.y);

            perimeter_point.x = start.x + delta.x;
            perimeter_point.y = start.y + delta.y;

            i32 perim_i = perimeter_point.y * col_len + perimeter_point.x;

            /* -- Skip neighbour if: tile is not visible -- */
            if (!Pathfinding_Tile_Visible(sightmap, blockmap, start, delta, col_len)) {
                continue;
            }

            /* -- Update sightmap -- */
            sightmap[perim_i] = SIGHTMAP_VISIBLE;
        }
    }

}

i32 *Pathfinding_Visible(i32 *blockmap, size_t row_len, size_t col_len,
                         struct Point start, i32 sight) {
    i32 *sightmap = calloc(row_len * col_len, sizeof(i32));
    Pathfinding_Visible_noM(sightmap, blockmap, row_len, col_len, start, sight);

    return (sightmap);
}
