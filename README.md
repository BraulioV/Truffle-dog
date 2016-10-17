# Truffle-dog
## A simple IA that picks truffles from a map before they gets rotten to score more points implemented for Artificial Intelligence subject of UGR in 2014-2015.

The part that I implemented is `agent.h` and `agent.cpp`, that is where the magic comes true. It's a simple AI that moves the dog in different directions to pick the truffles as big as possible, trying to get more points.

The agent implements a map of the world. This map is a matrix of $n\times n$ size, comoposed of `cell_data_`. `cell_data_` is a struct that stores if the cell is a wall or not, the size of the truffle (if there's a truffle in this cell), when was the last time that the cell was read (a cell is read when the agent has sniffed it) and when was the last time that the agent pass over that cell. This is used to build a map of our world and improves the search of our truffles.

Also, the agent has knowledge of the last action performed, his orientation (North: 0, East: 1, South:2 and West:3), if he hit a wall or not (`bump_` boolean), his position on the map, the truffle size of the cell and an interger that count the time to sniff on a `cell_data_`.

These are the actions that the agent can perform:
  
  * ___actFORWARD___: the agent will move forward.
  * ___actTURN_L___: the agent will turn to the left 90 degrees.
  * ___actTURN_R___: the same as before, but to the right.
  * ___actSNIFF___: the agent will sniff the ground to detect if there's a truffle or not.
  * ___actEXTRACT___: the agent will extract the truffle if there's one in that position. Otherwise, the agent cannot extract nothing;
  * ___actIDLE___: the agent will do nothing, just he will see his life passing in front of his eyes (or sensors);

The main methods of the agent are `Think` and `WhereToMove`. Let's start with `Think`:
  - `Think`: the purpouse of this method is to reason the next action of the agent from his state and the state of the current world. Firstly, it takes the last action and updates the state of the world and his own state too.
    * ___actFORWARD___: if this was the last action, it looks if he hit the wall or not. In case that the flag `bump_` is active, it means that he hit a wall, so he update the map and mark the cell as a wall. In other case, he will update it's position (coordinate X or Y), depending of the orientation that he has at that moment.
    * ___actTURN_L___ and ___actTURN_R___: both actions will produce an update of the orientation of the agent, increasing by one the orientation in case of *actTURN_R* and decreasing by one the orientation in case of *actTURN_L*.
    * ___actSNIFF___: update the current `cell_data_.size` to the `truffle_size` of the agent, to mark the size of the truffle on that position at the current moment.
  
    After that 
 
