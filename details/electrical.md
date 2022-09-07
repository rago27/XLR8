# Electrical

## Previous experience
As our team had previously done ITSP, we had some experience with electrical circuits and components for XLR8.
Hence, we did not struggle much and were able to design the circuit and make it relatively quickly.


## Initial Design
We had no experience working with the L293D motor driver, so decided to make the initial circuit with it (despite there being better, easier to 
implement motor drivers). After making the circuit using female headers, short wires and a lot of solder on a circuit board, we tested the circuit for electrical
correctness using a multimeter. Finally we put all the electronics on it, integrated the board with the chassis and drove our bot around using it.


## Motors
We'd decided not to use the motors provided by ERC to all the teams are they were not of very good quality and we anyways needed only 2 motors :).
So, we got 2 200 rpm Johnson motors which have much more torque and much less resistance on the shaft when off (which actually turned out to be a disadvantage 
during the runs on the course).
These motors actually draw more current than the L293D can provide, when stalled. Thus, we switched to the L298N, which is rated for the higher currents that we required 
and for safety.

## Final Design
The final circuit only contained the ESP32
Only esp and l298n

simplicity
