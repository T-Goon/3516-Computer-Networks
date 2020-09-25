#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEQ0 0
#define SEQ1 1
#define ACK 0
#define NAK 1

struct pkt* makePacket(int seqnum, int acknum, int checksum,
  char* payload);

int isCorrupt(struct pkt* packet);

int calculateChecksum(char* vdata, int acknum, int seqnum);
