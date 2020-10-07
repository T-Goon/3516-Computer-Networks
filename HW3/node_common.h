
struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};


void printdt( int MyNodeNumber, struct NeighborCosts *neighbor,
   struct distance_table *dtptr);
