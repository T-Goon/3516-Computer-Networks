#include <stdio.h>
#include "project3.h"
#include "node_common.h"

#define NODE_NUM 0

extern int TraceLevel;

struct distance_table dt0;
struct NeighborCosts   *neighbor0;

/* students to write the following two routines, and maybe some others */

void rtinit0() {
  neighbor0 = getNeighborCosts(NODE_NUM);
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {

}
