#include "node_common.h"

#define NODE_NUM 2

extern int TraceLevel;
extern float clocktime;

struct distance_table dt2;
struct NeighborCosts   *neighbor2;

/* students to write the following two routines, and maybe some others */

void rtinit2() {
  // Get initial neighbor costs
  neighbor2 = getNeighborCosts(NODE_NUM);

  // Initialize distance table
  int temp[MAX_NODES][MAX_NODES] = {
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {neighbor2->NodeCosts[0], neighbor2->NodeCosts[1], neighbor2->NodeCosts[2], neighbor2->NodeCosts[3]},
    {INFINITY, INFINITY, INFINITY, INFINITY}
  };

  memcpy(&dt2.costs, &temp, sizeof(int) * MAX_NODES * MAX_NODES);

  initMsg(NODE_NUM, &dt2, clocktime);

  // Broadcast current nodes distances to neighbors
  broadcast(NODE_NUM, neighbor2, &dt2, clocktime);
}


void rtupdate2( struct RoutePacket *rcvdpkt ) {
  rcvMsg(NODE_NUM, rcvdpkt->sourceid, clocktime);

  // Update this node's distance table
  int updated = updateDt(NODE_NUM, &dt2, rcvdpkt->sourceid, rcvdpkt->mincost);

  // Chck if distance table has been updated
  if(updated == YES){
    currentDTmsg(NODE_NUM, &dt2, clocktime);

    // If yes broadcast distance table to neighbors
    broadcast(NODE_NUM, neighbor2, &dt2, clocktime);
  }
}
