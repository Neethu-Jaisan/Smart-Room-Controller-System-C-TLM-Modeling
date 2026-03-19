#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "desi.h"

using namespace sc_core;
using namespace tlm;
using namespace std;

// ================= INTERCONNECT =================
struct Interconnect : sc_module {

    tlm_utils::simple_target_socket<Interconnect> cpu_socket;
    tlm_utils::simple_initiator_socket<Interconnect> room_socket;

    SC_CTOR(Interconnect)
        : cpu_socket("cpu_socket"), room_socket("room_socket")
    {
        cpu_socket.register_b_transport(this, &Interconnect::b_transport);
    }

    void b_transport(tlm_generic_payload& trans, sc_time& delay)
    {
        sc_dt::uint64 addr = trans.get_address();

        // Simple address decode (only one peripheral here)
        if (addr <= 0x14) {
            room_socket->b_transport(trans, delay);
        }
        else {
            trans.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
        }
    }
};

// ================= PROCESSOR =================
#define TEMP_ADDR     0x00
#define HUM_ADDR      0x04
#define MOTION_ADDR   0x08
#define FAN_ADDR      0x0C
#define LIGHT_ADDR    0x10
#define COLOR_ADDR    0x14

struct Processor : sc_module {

    tlm_utils::simple_initiator_socket<Processor> socket;

    SC_CTOR(Processor) : socket("socket") {
        SC_THREAD(run);
        srand(time(0));
    }

    void write(int addr, int value) {

        tlm_generic_payload trans;
        sc_time delay = SC_ZERO_TIME;

        trans.set_command(TLM_WRITE_COMMAND);
        trans.set_address(addr);
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(&value));
        trans.set_data_length(sizeof(int));

        socket->b_transport(trans, delay);
    }

    int read(int addr) {

        int value = 0;

        tlm_generic_payload trans;
        sc_time delay = SC_ZERO_TIME;

        trans.set_command(TLM_READ_COMMAND);
        trans.set_address(addr);
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(&value));
        trans.set_data_length(sizeof(int));

        socket->b_transport(trans, delay);
        return value;
    }

    void run() {

        for (int i = 0; i < 5; i++) {

            wait(20, SC_NS);

            int temp = 20 + rand() % 20;
            int hum  = 40 + rand() % 50;
            int motion = rand() % 2;

            write(TEMP_ADDR, temp);
            write(HUM_ADDR, hum);
            write(MOTION_ADDR, motion);

            wait(5, SC_NS);

            int fan   = read(FAN_ADDR);
            int light = read(LIGHT_ADDR);
            int color = read(COLOR_ADDR);

            cout << "\nProcessor Observation:\n";
            cout << " Fan   : " << fan
                 << " | Light: " << light
                 << " | Mode: " << (color ? "Warm" : "Cool") << "\n";
        }
    }
};

// ================= MAIN =================
int sc_main(int argc, char* argv[])
{
    SmartRoom room("SmartRoom");
    Interconnect bus("APB_Interconnect");
    Processor cpu("Processor");

    cpu.socket.bind(bus.cpu_socket);
    bus.room_socket.bind(room.socket);

    sc_start();
    return 0;
}
