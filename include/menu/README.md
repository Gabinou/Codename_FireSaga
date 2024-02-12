# Menu

Main point of interaction between Player and game

## Design objectives
- Show selection of actions Player can take
- Prompt for next action
- Player must make the next choice
- Show information to player
- Whatever is necessary to make choice of next action
- Show meta - information to the player
- e.g. How does the STR stat work

## Implementation details
- Arbitrary of elements (* **elem ***)
    , boxes on menu
    - Cursor can move between each ***elem ***using 4 directions
    - Each ***elem ***linked to 4 next ***elem ***using array of directions
    - Try to make each ***elem ***box fit snuggly in menu space
    - All ***elem *** + attributes are independent from menu appearance
    - Position, size, directions, links...
    - Press button with cursor on ***elem ***to perform action
    - Behavior taken charge by Game State FSM -> Menu FSM