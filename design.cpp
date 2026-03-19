#include "desi.h"
#include <iostream>

using namespace std;

#define TEMP_ADDR     0x00
#define HUM_ADDR      0x04
#define MOTION_ADDR   0x08
#define FAN_ADDR      0x0C
#define LIGHT_ADDR    0x10
#define COLOR_ADDR    0x14

SmartRoom::SmartRoom(sc_module_name name)
    : sc_module(name), socket("socket"),
      temp(0), hum(0), motion(0),
      fan_speed(0), light_state(0), light_color(0)
{
    socket.register_b_transport(this, &SmartRoom::b_transport);
}

void SmartRoom::process_logic() {

    if (motion == 0) fan_speed = 0;
    else if (temp > 28) fan_speed = 3;
    else if (hum > 70) fan_speed = 2;
    else fan_speed = 1;

    light_state = (motion == 1) ? 1 : 0;
    light_color = (temp > 28) ? 0 : 1;

    //  CLEAN OUTPUT FORMAT
    cout << "\n=========== SMART ROOM CYCLE ===========\n";

    cout << "INPUT:\n";
    cout << " Temp      : " << temp << " °C\n";
    cout << " Humidity  : " << hum  << " %\n";
    cout << " Occupancy : " << (motion ? "Present" : "Absent") << "\n";

    cout << "\nPROCESS:\n";
    cout << " Rule Engine Activated\n";

    cout << "\nOUTPUT:\n";

    cout << " Fan       : ";
    if (fan_speed == 0) cout << "OFF\n";
    else if (fan_speed == 1) cout << "LOW\n";
    else if (fan_speed == 2) cout << "MEDIUM\n";
    else cout << "HIGH\n";

    cout << " Light     : " << (light_state ? "ON" : "OFF") << "\n";
    cout << " Mode      : " << (light_color ? "Warm" : "Cool") << "\n";

    cout << "========================================\n";
}

void SmartRoom::b_transport(tlm_generic_payload& trans, sc_time& delay)
{
    int* data = reinterpret_cast<int*>(trans.get_data_ptr());
    sc_dt::uint64 addr = trans.get_address();

    if (trans.get_command() == TLM_WRITE_COMMAND) {

        if (addr == TEMP_ADDR) temp = data[0];
        else if (addr == HUM_ADDR) hum = data[0];
        else if (addr == MOTION_ADDR) {
            motion = data[0];
            process_logic();
        }
    }

    else if (trans.get_command() == TLM_READ_COMMAND) {

        if (addr == FAN_ADDR) data[0] = fan_speed;
        else if (addr == LIGHT_ADDR) data[0] = light_state;
        else if (addr == COLOR_ADDR) data[0] = light_color;
    }

    delay += sc_time(10, SC_NS);
    trans.set_response_status(TLM_OK_RESPONSE);
}
