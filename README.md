# Smart-Room-Controller-System-C-TLM-Modeling
## Electronic System LevelProject


## Overview

This project models a **Smart Room System-on-Chip (SoC)** using **SystemC and TLM-2.0**.
The design follows a processor–interconnect–peripheral architecture, where a processor communicates with a smart room controller through an **APB-like memory-mapped protocol**.

The system processes environmental inputs such as **temperature, humidity, and occupancy**, and dynamically controls:

* 🌪 Fan speed
* 💡 Light state (ON/OFF)
* 🎨 Light mode (Warm/Cool)

The model demonstrates how system-level behavior can be designed and validated before RTL implementation.

---

## System Architecture

```
Processor (Initiator / Master)
        ↓
APB-like Interconnect (Address Decoder)
        ↓
Smart Room Controller (Target / Peripheral)
        ↓
Fan + Light + Display Logic
```


## Memory Map (APB-Style)

| Address | Description            |
| ------- | ---------------------- |
| 0x00    | Temperature Input      |
| 0x04    | Humidity Input         |
| 0x08    | Motion / Occupancy     |
| 0x0C    | Fan Speed Output       |
| 0x10    | Light State            |
| 0x14    | Light Mode (Warm/Cool) |

---

## Functional Logic

### Fan Control Priority

1. No occupancy → Fan OFF
2. Temperature > threshold → High speed
3. Humidity > threshold → Medium speed
4. Otherwise → Low speed

### Lighting Control

* Occupancy present → Light ON
* No occupancy → Light OFF

### Light Mode

* High temperature → Cool mode
* Comfortable environment → Warm mode


---

## Simulation Flow

1. Processor generates random environmental inputs
2. Inputs are sent via TLM write transactions
3. Interconnect decodes address and forwards transaction
4. Controller processes logic upon receiving complete data
5. Outputs are updated and returned via read transactions

---


## Simulation Result

Example output from randomized testbench:
<img width="394" height="642" alt="image" src="https://github.com/user-attachments/assets/b8720538-b172-4971-8608-2f4a768e7875" />

---

## Limitations

* Uses abstracted protocol (not cycle-accurate APB)
* Processor is functionally modeled (not RTL CPU)
* Single peripheral (can be extended)

---

## Future Enhancements

* Add multiple peripherals (HVAC, display, sensors)
* Introduce non-blocking transport (`nb_transport`)
* Integrate simple RISC-V core (optional)
* Add real APB protocol signals
* Extend to RTL implementation
----
