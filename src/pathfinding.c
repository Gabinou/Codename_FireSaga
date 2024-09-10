
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
i32 *Taxicab_Circle_List(i32 *darr_list, i32 *matrix, i32 value, i32 x, i32 y,
                         size_t row_len, size_t col_len, struct Range *range) {
    // Taxicabs can't move diagonal so manhattan distance: abs(x1-x2) + abs(y1-ys2)
    // Manhattan (distance) used to trace 'circles' on square tilemap
    // Returns: List points at distance [range_min, range_max] dist from [x, y]
    Taxicab_Circle(matrix, value, x, y, row_len, col_len, range);
    darr_list = matrix2list_noM(matrix, darr_list, row_len, col_len);
    return (darr_list);
}

/* Draw values in taxicab circle between min and max range, centered on [x, y] */
void Taxicab_Circle(i32 *matrix, i32 value, i32 x, i32 y, size_t row_len, size_t col_len,
                    struct Range *range) {
    i32 subrangey_min, subrangey_max;
    for (i32 rangex = 0; rangex <= range->max; rangex++) {
        subrangey_min = (rangex > range->min) ? 0 : (range->min - rangex);
        subrangey_max = (rangex > range->max) ? 0 : (range->max - rangex);
        for (i32 rangey = subrangey_min; rangey <= subrangey_max; rangey++) {
            for (i32 n = 0; n < SQUARE_NEIGHBOURS; n++) {
                // TODO int_inbounds might break conditions! REMOVE
                i32 tempx = int_inbounds(x + q_cycle4_pmmp(n) * rangex, 0, col_len - 1);
                i32 tempy = int_inbounds(y + q_cycle4_ppmm(n) * rangey, 0, row_len - 1);
                matrix[tempy * col_len + tempx] = value;
            }
        }
    }
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
    i32 *block_ptr         = &direction_block.right;
    i32 *pushpullto_ptr    = &pushpullto.right;
    for (size_t i = 0; i < row_len; i++) {
        pushpulltomap[i] = PUSHPULLMAP_BLOCKED;

        pushpulltomap[start.y * col_len + start.x] = 0;
        for (i32 n = 0; n < NMATH_SQUARE_NEIGHBOURS; n++) {
            temp_distance = *(block_ptr + n);
            if (*(pushpullto_ptr + n) >= NMATH_PUSHPULLMAP_MINDIST) {
                for (i32  distance = 1; distance < temp_distance; distance++) {
                    // TODO int_inbounds might break conditions! REMOVE
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
                                                     size_t col_len, i32 range[2], struct Point target) {
    struct SquareNeighbours Pushto = {0, 0, 0, 0};
    struct Point neighbour;
    for (i32 distance = range[0]; distance <= range[1]; distance++) {
        for (i32  i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
            // TODO int_inbounds might break conditions! REMOVE
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
    i32 *block_ptr = &direction_block.right;
    i32 *pushpullto_ptr = &pushpullto.right;
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
                // TODO int_inbounds might break conditions! REMOVE
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

struct SquareNeighbours pathfinding_Direction_Block(i32 *costmap_pushpull, size_t row_len,
                                                    size_t col_len, struct Point start) {
    struct SquareNeighbours  distance_block = {0, 0, 0, 0};
    i32 *distance_ptr = &distance_block.right;
    struct Point neighbour = {0, 0};
    i32  distance = 0;
    while ((distance_block.top == 0) || (distance_block.bottom == 0) || (distance_block.left == 0)
           || (distance_block.right == 0)) {
        distance++;
        for (i32  i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
            // TODO int_inbounds might break conditions! REMOVE
            neighbour.x = int_inbounds((start.x + q_cycle4_pzmz(i) * distance), 0, col_len - 1);
            neighbour.y = int_inbounds((start.y + q_cycle4_zpzm(i) * distance), 0, row_len - 1);
            if ((costmap_pushpull[neighbour.y * col_len + neighbour.x] == 0)
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

/* --- Closest --- */
// Find tile with the lowest distance next to a blocked tile, EXCLUDING target
//  - ALSO should be connected to blocked target
struct Point Pathfinding_Closest_Unblocked(i32 *distmap, size_t row_len, size_t col_len,
                                           struct Point start, struct Point target) {
    SDL_assert((target.x >= 0) && (target.x < col_len));
    SDL_assert((target.y >= 0) && (target.y < row_len));
    SDL_assert((start.x  >= 0) && (start.x  < col_len));
    SDL_assert((start.y  >= 0) && (start.y  < row_len));

    int min_dist = row_len + col_len + 2;
    /* Frontier points queue, by priority. */
    /* Lowest priority (movcost + distance) is top priority, at low index. */
    struct Nodeq *frontier_queue = DARR_INIT(frontier_queue, struct Nodeq, row_len * col_len);
    struct Nodeq current = {.x = target.x, .y = target.y, .cost = 0, .priority = 0};
    struct Nodeq neighbour;
    DARR_PUT(frontier_queue, current);
    struct Point out = {-1, -1};

    while (DARR_NUM(frontier_queue) > 0) {
        current = DARR_POP(frontier_queue);

        /* - End loop when target point is reached - */
        // SHOULD NOT HAPPEND -> target is blocked
        if ((current.x == target.x) && (current.y == target.y))
            break;

        /* -- Visit all square neighbours -- */
        for (size_t n = 0; n < SQUARE_NEIGHBOURS; n++) {
            /* Get next neighbour movement cost */
            // TODO int_inbounds might break conditions! REMOVE
            neighbour.x     = int_inbounds(q_cycle4_mzpz(n) + current.x, 0, col_len - 1);
            neighbour.y     = int_inbounds(q_cycle4_zmzp(n) + current.y, 0, row_len - 1);
            int current_n = neighbour.y * col_len + neighbour.x;

            /* Compute conditions for adding neighbour to frontier */
            /* Skip neighbour if: not blocked */
            if (distmap[current_n] >= MOVEMAP_MOVEABLEMIN) {
                int dist = distmap[current_n];
                if (dist < min_dist) {
                    dist = min_dist;
                    out.x = neighbour.x;
                    out.y = neighbour.y;
                }
                continue;
            }

            /* Skip neighbour if: already visited AND higher cost */
            b32 visited = neighbour.priority;
            if (visited)
                continue;

            neighbour.priority = 1;
            DARR_INSERT(frontier_queue, neighbour, 0);
        }
    }

    DARR_FREE(frontier_queue);
    return (out);
}

struct Point Pathfinding_Closest_Unblocked_Manhattan(i32 *costmap, size_t row_len, size_t col_len,
                                                     struct Point target) {
    SDL_assert(target.x >= 0);
    SDL_assert(target.x < col_len);
    SDL_assert(target.y >= 0);
    SDL_assert(target.y < row_len);

    /* Return target if unblocked */
    if (costmap[target.y * col_len + target.x])
        return (target);

    /* Target is blocked */
    struct Point out = {-1, -1};
    size_t min_dist = row_len + col_len + 1;
    for (u32 i = 0; i < row_len * col_len; i++) {
        struct Point pnt = {i % col_len, i / col_len};
        i32 dist = _Pathfinding_Manhattan(target.x, target.y, pnt.x, pnt.y);
        if (dist < min_dist)
            out = pnt;
    }
    return (out);
}

/* --- Distance --- */
// Computes distance to walk to tile using costmap
// - Special rules to copmpute distance through walls
//      - Find tiles next to closest/thinnest walls
void Pathfinding_Distance(i32 *distmap, i32 *costmap, size_t row_len, size_t col_len,
                          struct Point target, struct Point stop) {
    Pathfinding_Distance_Plus(distmap, costmap, NULL, row_len, col_len, target, stop);
}

void Pathfinding_Distance_Plus(i32 *distmap, i32 *costmap, tnecs_entity *enemy_occupymap,
                               size_t row_len, size_t col_len,
                               struct Point target, struct Point stop) {
    SDL_assert((target.x >= 0) && (target.x < col_len));
    size_t bytesize = row_len * col_len * sizeof(*distmap);
    memset(distmap, 0, bytesize);

    /* Frontier points queue, by priority. */
    /* Lowest priority (movcost + distance) is top priority, at low index. */
    struct Nodeq *frontier_queue = DARR_INIT(frontier_queue, struct Nodeq, row_len * col_len);
    struct Nodeq current = {.x = target.x, .y = target.y};
    struct Nodeq neighbour;
    DARR_PUT(frontier_queue, current);

    while (DARR_NUM(frontier_queue) > 0) {
        current = DARR_POP(frontier_queue);
        if ((current.x == stop.x) && (current.y == stop.y))
            break;

        int current_n = current.y * col_len + current.x;

        /* -- Visit all square neighbours -- */
        for (size_t n = 0; n < SQUARE_NEIGHBOURS; n++) {
            /* Get next neighbour movement cost */
            // TODO int_inbounds might break conditions! REMOVE
            neighbour.x     = int_inbounds(q_cycle4_mzpz(n) + current.x, 0, col_len - 1);
            neighbour.y     = int_inbounds(q_cycle4_zmzp(n) + current.y, 0, row_len - 1);

            /* Skip neighbour if: is target */
            b32 istarget = ((target.x == neighbour.x) && (target.y == neighbour.y));
            if (istarget)
                continue;

            i32 neighbour_n = neighbour.y * col_len + neighbour.x;

            /* Compute conditions for adding neighbour to frontier */
            /* Compute distance from costmap */
            i32 dist;
            if (costmap[neighbour_n] < MOVEMAP_MOVEABLEMIN) {
                // DISTMAP_BLOCKED bigger than any distance travelled
                //  -> Always know which tiles are blocked
                //  -> Still can be used to compute distance through walls
                //  -> Very low priority. If any open region exists beyond the wall,
                //     only the closest trajectory will be opened
                dist = distmap[current_n] + DISTMAP_BLOCKED + 1;
            } else {
                //  If tile is unblocked, compute distance by removing blocked component
                //  -> Compute distance even through walls
                dist = (distmap[current_n] % DISTMAP_OCCUPIED) + costmap[neighbour_n];
            }

            /* Compute distance from occupymap */
            // short-circuit on NULL occupymap
            // Overwrites distmap, higher priority than walls
            if ((enemy_occupymap != NULL) && (enemy_occupymap[neighbour_n] != TNECS_NULL))
                dist = distmap[current_n] + DISTMAP_OCCUPIED + 1;

            /* Skip neighbour if: already visited AND higher cost AN */
            b32 higher_cost        = (dist >= distmap[neighbour_n]);
            b32 visited            = (distmap[neighbour_n] > 0);
            if (visited && higher_cost)
                continue;

            /* Find index to insert neighbour into priority queue. */
            distmap[neighbour_n] = dist;

            /* Djikstra algo only has cost in this step */
            neighbour.priority = dist;

            /* Find index to insert neighbour into priority queue. */
            frontier_queue = Pathfinding_Frontier_Insert(frontier_queue, neighbour);
        }
    }

    /* Reset occupied and blocked tiles to 0 (blocked) */
    for (size_t i = 0; i < row_len * col_len; i++) {
        if (distmap[i] > DISTMAP_OCCUPIED)
            distmap[i] = COSTMAP_BLOCKED;
    }
    DARR_FREE(frontier_queue);
}

struct Nodeq *Pathfinding_Frontier_Insert(struct Nodeq *frontier_queue, struct Nodeq insert) {
    size_t index = DARR_NUM(frontier_queue);
    while ((index > 0) && (insert.priority > frontier_queue[index - 1].priority))
        index--;

    DARR_INSERT(frontier_queue, insert, index);
    return (frontier_queue);
}

/* Astar pathfinding + occupiable tiles, moving n tiles accross path to target */
i32 *Pathfinding_Astar_plus(i32 *path_list, i32 *costmap, tnecs_entity *occupymap,
                            size_t row_len, size_t col_len, int move,
                            struct Point start, struct Point end, b32 forward) {
    /* Assumes square grid, path_list is a DARR */

    // entity_print(occupymap, row_len, col_len);

    /* [1]: http://www.redblobgames.com/pathfinding/a-star/introduction.html */
    /* -- Checks -- */
    SDL_assert((start.x != end.x) || (start.y != end.y));
    if (costmap[start.y * col_len + start.x] < MOVEMAP_MOVEABLEMIN) {
        SDL_LogError(SOTA_LOG_AI, "Pathfinding: start position not moveable.");
    }

    /* Alloc variables */
    i32 *cost       = calloc(row_len * col_len, sizeof(*cost));
    i32 *came_from  = calloc(row_len * col_len, sizeof(*came_from));
    i32 *distmap    = calloc(row_len * col_len, sizeof(*distmap));

    Pathfinding_Distance(distmap, costmap, row_len, col_len, end, start);

    // matrix_print(distmap, row_len, col_len);
    struct Point closest = start; /* In case target is blocked */
    struct Nodeq current = {.x = start.x, .y = start.y, .cost = 0};
    struct Nodeq neighbour;

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
        // Note:    Use Manhnattan distance to find closest tile,
        //          cause tiles close to end might be blocked
        int current_dist = distmap[current.y * col_len + current.x];
        int closest_dist = distmap[closest.y * col_len + closest.x];

        int index = current.y * col_len + current.x;
        b32 occupied = (occupymap == NULL) ? false : (occupymap[index] != TNECS_NULL);

        b32 closest_valid   = (closest_dist >= MOVEMAP_MOVEABLEMIN);
        b32 current_valid   = (current_dist >= MOVEMAP_MOVEABLEMIN);
        b32 closer          = (current_dist < closest_dist);

        b32 cond1           = current_valid && !closest_valid && !occupied;
        b32 cond2           = current_valid &&  closest_valid && closer && !occupied;
        if (cond1 || cond2) {
            closest.x = current.x;
            closest.y = current.y;
        }

        /* -- Visit all square neighbours -- */
        for (size_t n = 0; n < SQUARE_NEIGHBOURS; n++) {
            /* Get next neighbour movement cost */
            // TODO int_inbounds might break conditions! REMOVE
            neighbour.x     = int_inbounds(q_cycle4_mzpz(n) + current.x, 0, col_len - 1);
            neighbour.y     = int_inbounds(q_cycle4_zmzp(n) + current.y, 0, row_len - 1);
            int current_n   = neighbour.y * col_len + neighbour.x;
            neighbour.cost  = current.cost + costmap[current_n];

            /* Compute conditions for adding neighbour to frontier */
            /* Skip neighbour if: blocked */
            if (costmap[current_n] < MOVEMAP_MOVEABLEMIN) {
                continue;
            }

            /* Skip neighbour if: blocked */
            if (distmap[current_n] < MOVEMAP_MOVEABLEMIN) {
                continue;
            }

            /* Skip neighbour if: cost greater than move */
            if ((neighbour.cost > move) && (move >= 0)) {
                continue;
            }

            /* Skip neighbour if: tile is occupied at MOVE */
            // short-circuit on NULL occupymap
            // Occupied tiles can be TRAVERSED but not OCCUPIED.
            // What does that mean for frontier? Nothing. Tile CAN be in frontier.
            if ((occupymap != NULL) && (occupymap[current_n] != TNECS_NULL) && (neighbour.cost == move)) {
                continue;
            }

            /* Skip neighbour if: already visited AND higher cost */
            b32 higher_cost  = (neighbour.cost >= cost[current_n]);
            b32 visited      = (cost[current_n] > 0);
            if (visited && higher_cost) {
                continue;
            }

            /* distance is heuristic for closeness to goal */
            cost[current_n] = neighbour.cost;

            /* Djikstra algo only has cost in this step */
            neighbour.priority = neighbour.cost + distmap[current_n];

            /* Find index to insert neighbour into priority queue. */
            frontier_queue = Pathfinding_Frontier_Insert(frontier_queue, neighbour);

            // DARR_INSERT(frontier_queue, neighbour, index);
            struct Point movep = {neighbour.x - current.x, neighbour.y - current.y};

            /* Update came_from, to build better path */
            // Occupied tiles can be TRAVERSED but not OCCUPIED.
            // What does it mean for came from?
            // came from from SHOULD NOT lead to occupied tile
            came_from[current_n] = Ternary_Direction(movep);
        }
    }

    PathList_f forward_f    = &Pathfinding_PathList_Forward;
    PathList_f backward_f   = &Pathfinding_PathList_Backward;
    PathList_f pathlist_f   = forward ? forward_f : backward_f;

    if ((current.x == end.x) && (current.y == end.y)) {
        /* End reached, get path to it*/
        int current_n = end.y * col_len + end.x;
        if (occupymap != NULL)
            SDL_assert(occupymap[current_n] == TNECS_NULL);
        path_list = pathlist_f(path_list, came_from, col_len, start, end);
    } else {
        /* End NOT reached, get path to closest tile */
        int current_n = closest.y * col_len + closest.x;
        /* If closest is equal to start: don't move. */
        if ((closest.x != start.x) || (closest.y != start.y)) {
            if (occupymap != NULL)
                SDL_assert(occupymap[current_n] == TNECS_NULL);
        }
        path_list = pathlist_f(path_list, came_from, col_len, start, closest);
    }

    SDL_free(cost);
    SDL_free(came_from);
    SDL_free(distmap);
    DARR_FREE(frontier_queue);

    return (path_list);
}

i32 *Pathfinding_Astar(i32 *path_list, i32 *costmap, size_t row_len, size_t col_len,
                       struct Point start, struct Point end, b32 forward) {
    path_list = Pathfinding_Astar_plus(path_list, costmap, NULL,
                                       row_len, col_len, INT_MAX,
                                       start, end, forward);
    return (path_list);
}

// Note: Pathfinding_Moveto finds traversable tiles only, without checking for occupied tiles
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
    b32 neighbour_inclosed;

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
        // TODO int_inbounds might break conditions! REMOVE
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

/* -- Attackfrom -- */
void Pathfinding_Attackfrom_noM(i32 *attackmap, i32 *movemap,
                                size_t row_len, size_t col_len,
                                struct Point target, i32 range[2]) {
    /* -- Wipe attackmap -- */
    for (i32 i = 0; i < row_len * col_len; i++)
        attackmap[i] = ATTACKMAP_BLOCKED;

    /* -- Trace range around target position -- */
    struct Range Srange = {range[0], range[1]};
    Taxicab_Circle(attackmap, 1, target.x, target.y,
                   row_len, col_len, &Srange);

    /* -- Remove non-traversable tiles -- */
    for (size_t i = 0; i < col_len * row_len; i++)
        attackmap[i] *= movemap[i];
}

/* -- Attackto -- */
// Where can aggressor attack to considering where he can move?
//  - Output does not use movement cost. Uses manhattan distance.
//      - Useful for auras and supports as well
//  - Note: Overwrites attackmap a couple times -> can't find which weapon
//          can be used to attackto
void Pathfinding_Attackto_noM(i32 *attackmap, i32 *move_matrix,
                              u64 *occupymap,
                              size_t row_len, size_t col_len,
                              i32 range[2], i32 mode_movetile) {
    /* -- Wipe attackmap -- */
    for (i32 i = 0; i < row_len * col_len; i++) {
        attackmap[i] = ATTACKMAP_BLOCKED;
    }

    /* -- Setup variables -- */
    i32 *move_list  = matrix2list(move_matrix, row_len, col_len);
    size_t list_len = DARR_NUM(move_list) / TWO_D;

    /* -- For every point in movemap -- */
    for (i32 i = 0; i < list_len; i++) {
        i32 x = move_list[i * TWO_D + 0];
        i32 y = move_list[i * TWO_D + 1];

        /* Can only attack from tile if not occupied */
        if ((occupymap != NULL) &&
            (occupymap[y * col_len + x] > TNECS_NULL)) {
            continue;
        }

        // NOTE:    This call may overwrite attackmap distances
        //          Can't find which weapon was used to attack.
        _Pathfinding_Attackto(x, y, attackmap, move_matrix, occupymap,
                              row_len, col_len, range, mode_movetile);
    }
    DARR_FREE(move_list);
}

i32 *Pathfinding_Attackto(i32 *move_matrix, u64 *occupymap, size_t row_len, size_t col_len,
                          i32 range[2], i32 mode_movetile) {
    /* -- Setup output attackmap -- */
    i32 *attackmap = calloc(row_len * col_len, sizeof(*attackmap));
    Pathfinding_Attackto_noM(attackmap, move_matrix, occupymap, row_len, col_len, range, mode_movetile);

    return (attackmap);
}

/* -- Pathfinding_Attackto_Neighbours -- */
// Compute manhattan distance to x,y point if it can be attacked from x,y.
// If move_matrix is NULL, effectively attackto only from (x,y) point.
void _Pathfinding_Attackto(i32 x, i32 y,
                           i32 *attackmap, i32 *move_matrix,
                           u64 *occupymap,
                           size_t row_len, size_t col_len,
                           i32 range[2], i32 mode_movetile) {
    /* -- Setup variables -- */
    struct Point point;
    b32 add_point  = (mode_movetile != MOVETILE_EXCLUDE);
    if (mode_movetile == MOVETILE_INCLUDE) {
        attackmap[y * col_len + x] = 1;
    }

    /* -- Iterate over possible ranges in x -- */
    for (i32 rangex = 0; rangex <= range[1]; rangex++) {
        i32 subrangey_min = (rangex >= range[0]) ? 0 : (range[0] - rangex);
        i32 subrangey_max = (rangex >= range[1]) ? 0 : (range[1] - rangex);
        /* -- Iterate over possible ranges in y, knowing x range -- */
        for (i32 rangey = subrangey_min; rangey <= subrangey_max; rangey++) {
            SDL_assert((rangex + rangey) >= range[0]);
            SDL_assert((rangex + rangey) <= range[1]);
            /* -- Iterate over range 4 combinations: x+y+, x+y-, x-y+, x-y- */
            for (i32 n = 0; n < SQUARE_NEIGHBOURS; n++) {
                point.x = x + q_cycle4_pmmp(n) * rangex;
                point.y = y + q_cycle4_ppmm(n) * rangey;

                /* Skip if point out of bounds */
                if ((point.x < 0) || (point.x >= col_len)) {
                    continue;
                }
                if ((point.y < 0) || (point.y >= col_len)) {
                    continue;
                }

                if (mode_movetile == MOVETILE_EXCLUDE) {
                    if (move_matrix == NULL) {
                        /*Add point only if different from start */
                        add_point = (point.y != y) || (point.x != x);
                    } else {
                        /* Add point if can't move to point */
                        b32 movemap_blocked = (move_matrix[point.y * col_len + point.x] == MOVEMAP_BLOCKED);
                        // If tile blocked by unit, it CAN be attacked
                        b32 unitmap_blocked = (occupymap == NULL) ? false : (occupymap[point.y * col_len + point.x] >
                                                                             TNECS_NULL);
                        add_point = movemap_blocked || unitmap_blocked;
                    }
                }

                /* Skip if not adding point to attackmap */
                if (!add_point)
                    continue;

                i32 val = abs(point.x - x) + abs(point.y - y);
                attackmap[point.y * col_len + point.x] = val > 0 ? val : 1;
            }
        }
    }

}

/* -- Unit Gradient -- */
void Pathfinding_unitGradient_noM(i32 *gradmap, i32 *costmap,
                                  size_t row_len, size_t col_len,
                                  struct Point *targets, size_t unit_num) {
    /* -- Wipe gradmap -- */
    for (i32 i = 0; i < col_len * row_len; i++) {
        if (costmap[i] < NMATH_PUSHPULLMAP_BLOCKED)
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
            // TODO int_inbounds might break conditions! REMOVE
            neighbour.x = int_inbounds(q_cycle4_mzpz(n) + current.x, 0, col_len - 1);
            neighbour.y = int_inbounds(q_cycle4_zmzp(n) + current.y, 0, row_len - 1);
            neighbour.distance = gradmap[current.y * col_len + current.x] + 1;
            int current_n = neighbour.y * col_len + neighbour.x;

            /* Skip neighbour if: blocked */
            if (costmap[current_n] < COSTMAP_MIN)
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

i32 *Pathfinding_unitGradient(i32 *costmap, size_t row_len, size_t col_len,
                              struct Point *targets, size_t unit_num) {
    i32 *gradmap = calloc(row_len * col_len, sizeof(i32));
    Pathfinding_unitGradient_noM(gradmap, costmap, row_len, col_len, targets, unit_num);

    return (gradmap);
}

/* -- Visible -- */
b32 Pathfinding_Tile_Visible(i32 *sightmap, i32 *blockmap, struct Point start,
                             struct Point delta, size_t col_len) {
    /* -- Between start and delta -- */
    i32 distance = abs(delta.x) + abs(delta.y);

    b32 visible = true;
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

            // TODO int_inbounds might break conditions! REMOVE
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
