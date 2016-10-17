# Truffle-dog
## A simple IA that picks truffles from a map before they gets rotten to score more points implemented for Artificial Intelligence subject of UGR in 2014-2015.

The part that I implemented is `agent.h` and `agent.cpp`, that is where the magic comes true. It's a simple AI that moves the dog in different directions to pick the truffles as big as possible, trying to get more points.

The agent implements a map of the world. This map is a matrix of $n\times n$ size, comoposed of `cell_data_`. `cell_data_` is a struct that stores if the cell is a wall or not, the size of the truffle (if there's a truffle in this cell), when was the last time that the cell was read (a cell is read when the agent has sniffed it) and when was the last time that the agent pass over that cell. This is used to build a map of our world and improves the search of our truffles.

Also, the agent has knowledge of the last action performed, his orientation (North: 0, East: 1, South:2 and West:3), if he hit a wall or not (`bump_` boolean), his position on the map, the truffle size of the cell.

These are the actions that the agent can perform:
  
  * ___actFORWARD___: the agent will move forward.
  * ___actTURN_L___: the agent will turn to the left 90 degrees.
  * ___actTURN_R___: the same as before, but to the right.
  * ___actSNIFF___: the agent will sniff the ground to detect if there's a truffle or not.
  * ___actEXTRACT___: the agent will extract the truffle if there's one in that position. Otherwise, the agent cannot extract nothing;
  * ___actIDLE___: the agent will do nothing, just he will see his life passing in front of his eyes (or sensors);
