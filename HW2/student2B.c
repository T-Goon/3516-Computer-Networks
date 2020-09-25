#include "project2.h"
#include "student_common.h"


int current_state;
/*
 * The following routine will be called once (only) before any other
 * entity B routines are called. You can use it to do any initialization
 */
void B_init() {
  current_state = SEQ0;
}
/*
 * B_input(packet),where packet is a structure of type pkt. This routine
 * will be called whenever a packet sent from the A-side (i.e., as a result
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side.
 * packet is the (possibly corrupted) packet sent from the A-side.
 */
void B_input(struct pkt packet) {

  if(isCorrupt(&packet)){
    char* empty = malloc(sizeof(MESSAGE_LENGTH));
    memset(empty, 0, MESSAGE_LENGTH);
    int checksum = calculateChecksum(empty, NAK, current_state);
    struct pkt* response = makePacket(packet->seqnum, NAK, );
  }

  struct msg* message = malloc(sizeof(message));
  memcpy(message->data, packet.payload, MESSAGE_LENGTH);
  tolayer5(BEntity, *message);

  free(message);
}

void B_output(struct msg message){

}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires
 * (thus generating a timer interrupt). You'll probably want to use this
 * routine to control the retransmission of packets. See starttimer()
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void  B_timerinterrupt() {
}
