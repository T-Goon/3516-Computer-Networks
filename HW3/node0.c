#include "node_common.h"

#define NODE_NUM 0

extern int TraceLevel;

struct distance_table dt0;
struct NeighborCosts *neighbor0;

/* students to write the following two routines, and maybe some others */

void rtinit0() {
  // Get initial neighbor costs
  neighbor0 = getNeighborCosts(NODE_NUM);

  // Initialize distance table
  int temp[MAX_NODES][MAX_NODES] = {
    {neighbor0->NodeCosts[0], neighbor0->NodeCosts[1], neighbor0->NodeCosts[2], neighbor0->NodeCosts[3]},
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY}
  };

  memcpy(&dt0.costs, &temp, sizeof(int) * MAX_NODES * MAX_NODES);

  //printdt(NODE_NUM, neighbor0, &dt0);
  printdtALL(NODE_NUM, &dt0);

  // Broadcast current nodes distances to neighbors
  broadcast(NODE_NUM, neighbor0, &dt0);
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {

}
