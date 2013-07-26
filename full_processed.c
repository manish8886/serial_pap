
struct transport {

  uint8_t payload[256];

  volatile uint8_t payload_len;

  volatile bool_t msg_received;

  uint8_t ovrn, error;
};
# 41 "pprz_transport.h" 2




extern uint8_t ck_a, ck_b;
# 142 "pprz_transport.h"
struct pprz_transport {

  struct transport trans;

  uint8_t status;
  uint8_t payload_idx;
  uint8_t ck_a, ck_b;
};

extern struct pprz_transport pprz_tp;

static inline void parse_pprz(struct pprz_transport * t, uint8_t c ) {
  switch (t->status) {
  case 0:
    if (c == 0x99)
      t->status++;
    break;
  case 1:
    if (t->trans.msg_received) {
      t->trans.ovrn++;
      goto error;
    }
    t->trans.payload_len = c-4;
    t->ck_a = t->ck_b = c;
    t->status++;
    t->payload_idx = 0;
    break;
  case 2:
    t->trans.payload[t->payload_idx] = c;
    t->ck_a += c; t->ck_b += t->ck_a;
    t->payload_idx++;
    if (t->payload_idx == t->trans.payload_len)
      t->status++;
    break;
  case 3:
    if (c != t->ck_a)
      goto error;
    t->status++;
    break;
  case 4:
    if (c != t->ck_b)
      goto error;
    t->trans.msg_received = TRUE;
    goto restart;
  default:
    goto error;
  }
  return;
 error:
  t->trans.error++;
 restart:
  t->status = 0;
  return;
}

static inline void pprz_parse_payload(struct pprz_transport * t) {
  uint8_t i;
  for(i = 0; i < t->trans.payload_len; i++)
    dl_buffer[i] = t->trans.payload[i];
  dl_msg_available = TRUE;
}
# 8 "test.c" 2

int main(){
  { if (i15 == 0) { { static uint8_t i; int16_t climb = -gps.ned_vel.z; int16_t course = (DegOfRad(gps.course)/((int32_t)1e6)); { if (UART1CheckFreeSpace((uint8_t)(((0+1+4+4+2+4+2+2+2+4+1+1 +2)+4)))) { 

	    downlink_nb_bytes += ((0+1+4+4+2+4+2+2+2+4+1+1 +2)+4);; 


	    { downlink_nb_msgs++; { UART1Transmit(0x99); uint8_t msg_len = ((0+1+4+4+2+4+2+2+2+4+1+1 +2)+4); UART1Transmit(msg_len); ck_a = msg_len; ck_b = msg_len; }; { ck_a += AC_ID; ck_b += ck_a; UART1Transmit(AC_ID); }; { ck_a += 8; ck_b += ck_a; UART1Transmit(8); }; }

	    
	    { uint8_t _x = *((const uint8_t*)(&gps.fix)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; 
	    


{ { { uint8_t _x = *((const uint8_t*)(&gps.utm_pos.east)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&gps.utm_pos.east)+1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; { { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&gps.utm_pos.east)+2); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(const uint8_t*)(&gps.utm_pos.east)+2 +1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; }; 


	    { { { uint8_t _x = *((const uint8_t*)(&gps.utm_pos.north)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&gps.utm_pos.north)+1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; { { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&gps.utm_pos.north)+2); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(const uint8_t*)(&gps.utm_pos.north)+2 +1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; }; 


	    { { uint8_t _x = *((const uint8_t*)(&course)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&course)+1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; 


	    { { { uint8_t _x = *((const uint8_t*)(&gps.hmsl)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&gps.hmsl)+1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; { { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&gps.hmsl)+2); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(const uint8_t*)(&gps.hmsl)+2 +1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; }; 

	    { { uint8_t _x = *((const uint8_t*)(&gps.gspeed)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&gps.gspeed)+1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; 


{ { uint8_t _x = *((const uint8_t*)(&climb)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&climb)+1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; 

	    { { uint8_t _x = *((const uint8_t*)(&gps.week)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&gps.week)+1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; 

	    
	    { { { uint8_t _x = *((const uint8_t*)(&gps.tow)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&gps.tow)+1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; { { uint8_t _x = *((const uint8_t*)(const uint8_t*)(&gps.tow)+2); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; { uint8_t _x = *((const uint8_t*)(const uint8_t*)(const uint8_t*)(&gps.tow)+2 +1); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; }; }; 

{ uint8_t _x = *((const uint8_t*)(&gps.utm_pos.zone)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; }; 

    { uint8_t _x = *((const uint8_t*)(&i)); { ck_a += _x; ck_b += ck_a; UART1Transmit(_x); }; };
 { UART1Transmit(ck_a); UART1Transmit(ck_b); UART1SendMessage(); } } else downlink_nb_ovrn++;; }; i++; };
    }
  }
}

