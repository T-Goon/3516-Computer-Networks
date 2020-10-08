#include "node_common.h"

#define NODE_NUM 3

extern int TraceLevel;

struct distance_table dt3;
struct NeighborCosts   *neighbor3;

/* students to write the following two routines, and maybe some others */

void rtinit3() {
  // Get initial neighbor costs
  neighbor3 = getNeighborCosts(NODE_NUM);

  // Initialize distance table
  int temp[MAX_NODES][MAX_NODES] = {
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {neighbor3->NodeCosts[0], neighbor3->NodeCosts[1], neighbor3->NodeCosts[2], neighbor3->NodeCosts[3]}
  };

  memcpy(&dt3.costs, &temp, sizeof(int) * MAX_NODES * MAX_NODES);

  //printdt(NODE_NUM, neighbor3, &dt3);
  printdtALL(NODE_NUM, &dt3);

  // Broadcast current nodes distances to neighbors
  broadcast(NODE_NUM, neighbor3, &dt3);
}


void rtupdate3( struct RoutePacket *rcvdpkt ) {

}
