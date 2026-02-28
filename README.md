# Smart Room Controller – TLM Based ESLD Project

## Overview

This project models a simple Smart Room Controller using SystemC and TLM-2.0.

The system reads:
- 🌡 Temperature
- 💧 Humidity
- 🧍 PIR Motion

Based on threshold conditions, the controller sets the appropriate fan speed.

The model demonstrates both:
- Blocking transport (sensor reads)
- Non-blocking transport (fan control)

---

## System Logic

Decision Priority:

1. If no motion → Fan OFF  
2. If temperature > 30°C → High Speed  
3. If humidity > 70% → Medium Speed  
4. Otherwise → Low Speed  

---

## Concepts Demonstrated

- Transaction-Level Modeling (TLM-2.0)
- Initiator and Target sockets
- Blocking (`b_transport`)
- Non-blocking (`nb_transport_fw`)
- Embedded control logic modeling
- Separation of design and testbench
- Random stimulus verification

---

## Simulation Result

Example output from randomized testbench:

![Simulation Results](systemc_fan(1).png)

The output shows:
- Sensor values
- Threshold comparisons
- Final fan state for each scenario

---

## Conclusion

This project demonstrates how embedded control systems can be modeled at the transaction level to evaluate system behavior and decision logic before RTL implementation.
