#include "node_common.h"

// Send the nodes min cost row to all neighbors
void broadcast(int sourceid, struct NeighborCosts* neighbors,
  struct distance_table* dt, float time){

  for(int i=0; i<MAX_NODES; i++){
    // Nodes that is not the sending node
    if (neighbors->NodeCosts[i] != 0 && neighbors->NodeCosts[i] != INFINITY){
      // Make a new packet
      struct RoutePacket pkt;
      pkt.sourceid = sourceid;
      pkt.destid = i;
      memcpy(&pkt.mincost, &dt->costs[sourceid], sizeof(int)*MAX_NODES);

      // Send packet
      toLayer2(pkt);

      sndMsg(pkt.destid, pkt.sourceid, dt, time);
    }
  }
}

// Update a node's distance table
int updateDt(int rcvId, struct distance_table* rcvDt,
  int sndId, int sndCosts[MAX_NODES]){
  int updtFlag = NO;

  // Copy the min costs of the sender into receiver table
  memcpy(&(rcvDt->costs[sndId]), sndCosts, sizeof(int)*MAX_NODES);

  // Apply the distance vector algorithm
  for(int i=0; i<MAX_NODES; i++){
    if(sndCosts[i]+rcvDt->costs[rcvId][sndId] <
      rcvDt->costs[rcvId][i]){
        rcvDt->costs[rcvId][i] = sndCosts[i]+rcvDt->costs[rcvId][sndId];

        updtFlag = YES;
      }
  }

  return updtFlag;
}

// Message to print out when the node initializes.
void initMsg(int id, struct distance_table* dt, float time){
  printf("At time t=%f, rtinit%d() called.\n", time, id);
  printf("At time t=%f, node %d initial distance vector: %d %d %d %d\n", time, id,
  dt->costs[id][0], dt->costs[id][1], dt->costs[id][2], dt->costs[id][3]);
}

// Message to print when node sends packet to another node
void sndMsg(int rcvid, int sndid, struct distance_table* dt, float time){
  printf("At time t=%f, node %d sends packet to node %d with: %d %d %d %d\n",
  time, sndid, rcvid, dt->costs[sndid][0], dt->costs[sndid][1], dt->costs[sndid][2],
  dt->costs[sndid][3]);
}

// Messgae to print when node receives a packet from another node
void rcvMsg(int rcvid, int sndid, float time){
  printf("At time t=%f, rtupdate%d() called, by a pkt received from Sender id: %d.\n",
  time, rcvid, sndid);
}

// Prints the current distance vector of the node.
void currentDTmsg(int id, struct distance_table* dt, float time){
  printf("At time t=%f, node %d current distance vector: %d %d %d %d\n",
  time, id, dt->costs[id][0], dt->costs[id][1], dt->costs[id][2], dt->costs[id][3]);
}

/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt( int MyNodeNumber, struct NeighborCosts *neighbor,
   struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt
