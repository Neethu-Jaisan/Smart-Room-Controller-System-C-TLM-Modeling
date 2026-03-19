#ifndef DESI_H
#define DESI_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

using namespace sc_core;
using namespace tlm;

struct SmartRoom : sc_module {

    tlm_utils::simple_target_socket<SmartRoom> socket;

    int temp, hum, motion;
    int fan_speed;
    int light_state;
    int light_color;

    SC_CTOR(SmartRoom);

    void b_transport(tlm_generic_payload& trans, sc_time& delay);
    void process_logic();
};

#endif
