# Truffle-dog
## A simple IA that picks truffles from a map before they gets rotten to score more points implemented for Artificial Intelligence subject of UGR in 2014-2015.

The part that I implemented is `agent.h` and `agent.cpp`, that is where the magic comes true. It's a simple AI that moves the dog in different directions to pick the truffles as big as possible, trying to get more points.

The agent implements a map of the world. This map is a matrix of $n\times n$ size, comoposed of `cell_data_`. `cell_data_` is a struct that stores if the cell is a wall or not, the size of the truffle (if there's a truffle in this cell), when was the last time that the cell was read (a cell is read when the agent has sniffed it) and when was the last time that the agent pass over that cell. This is used to build a map of our world and improves the search of our truffles.

Also, the agent has knowledge of the last action performed, his orientation (North: 0, East: 1, South:2 and West:3), if he hit a wall or not (`bump_` boolean), his position on the map, the truffle size of the cell and an interger that limit the time that the agent has to sniff on a `cell_data_`.

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
  
    After that, the agent will choose the next action, depending of the following conditions are satisfied or not:
    
    1. **The agent can sniff and the `time_to_smell_` is multiple of five**: when this condition is satisfied, the action is `actSNIFF`. If `time_to_smell_`is greater or equal to the `TIME` of the agent, the agent will try to estimate if the map is fast growth time of truffles map or not, and will update the parameter `can_i_smell_`to  `false`. This is use only to estimate the speed of the growth time of the truffle. Once is done, the agent is not allowed to sniff anymore.
    
    2. **The agent can't smell, the last action wasn't actTURN_L or actTURN_R, and the cell of agent's position is not read for a while**: if this is satisfied, the action will be `actEXTRACT`.
    
    3. **Otherwise**: the action will be to move, and this movement is calculated with `WhereToMove`.
    
  After that, the agent will update the last action, and increase the time of all the `cell_data.not_read_since_` of the map, and return the action.
  
 `WhereToMove` reasons the best action between `actFORWARD`, `actTURN_L` and `actTURN_R`, depending on how long ago the agent didn't go through a cell, if the agent has detect a wall or not and the differences of time between the cells
    
![start](/images/start.png) ![end](/images/end.png)
 
To compile the files, we need to install ***freeglut3-dev***.
