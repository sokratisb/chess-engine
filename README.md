# chess-engine
A multi-threaded chess engine written in C, using the alegro library for visuals and I/O.
- Install allegro 5 on your linux machine
- Download the source code
- Open a new terminal and cd to the repo you downloaded
- make all
- run the application with: ./chess Y, where Y is the number of threads

# Description
The engine only calculates one move ahead, giving priority to captures and favourable
exchanges. A single thread initializes a linked list for every piece in play, the items
of which are all the possible moves a piece can make. The threads then try to acquire
the lock of a move node, and calculate the strength of the move based on the following:
- Is the king in check ? Can this piece block ?
- Is the square-to-move-to protected ?
- Is the square occupied ? What is the value of the occupant ?
- Is it a protected square ? 
and so on.
After computing the move's strength, each thread tries to acquire the best-so-far move's
lock, then compares the best move with the current move and updates the best move if the
calculated strength is higher.

# Known issues
Threads (except thread 0) hang on game over

# Non implemented mechanics
En Passant
