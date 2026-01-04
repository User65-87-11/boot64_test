#pragma once

#include <stdint.h>


// typedef struct Command{
// 	uint8_t port;
// 	uint8_t data;

// }Command_t;


// extern Command_t  commands [5];

// void loop_send_set_len(uint32_t);
// void halt_loop_advance_comm();
// void loop_send_command(Command_t * cmd);

void halt_loop();
void halt_cli();