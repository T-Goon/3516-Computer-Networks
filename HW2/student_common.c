#include "student_common.h"
#include "project2.h"

/**
  Creates and returns a pointer to a new packet.
  seqnum: sequence number of the packet. (SEQ|SEQ1)
  acknum: acknowledgement type (ACK|NAK)
  checksum: checksum value of the packet.
  payload: the content of the packet
  return: pointer to a new packet filled in with given values.
*/
struct pkt* makePacket(int seqnum, int acknum, int checksum,
  char* payload){

  // Allocate memory for new packet
  struct pkt* packet = malloc(sizeof(struct pkt));

  // Fill in packet values
  packet->seqnum = seqnum;
  packet->acknum = acknum;
  packet->checksum = checksum;
  memcpy(packet->payload, payload, MESSAGE_LENGTH);

  return packet;
}

/**
  Determines if a packet is corrupt by checking checksum value.
  packet: pointer to the packet to check.
  return: TRUE of the packet is corrupt, FALSE otherwise.
*/
int isCorrupt(struct pkt* packet){
  int checksum = calculateChecksum(packet->payload, packet->acknum, packet->seqnum);

  if(packet->checksum == checksum)
    return FALSE;

  return TRUE;
}

// Calculates the checksum value.
int calculateChecksum(char* vdata, int acknum, int seqnum){
        int i, checksum = 0;

        for(i = 0; i < MESSAGE_LENGTH; i++){
          checksum += (int)(vdata[i]) * i;
        }

        checksum += acknum * 21;
        checksum += seqnum * 22;

        return checksum;
}

// Returns the opposite sequence number passed in.
// num: SEQ0 or SEQ1
int changeSEQ(int num){
  if(num == SEQ0)
    return SEQ1;
  else if(num == SEQ1)
    return SEQ0;
}
