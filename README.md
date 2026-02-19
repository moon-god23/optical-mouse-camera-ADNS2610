# optical-mouse-camera-ADNS2610
Turning a legacy ADNS-2610 mouse sensor into a low-res CMOS camera using Arduino and Python.
ADNS-2610 Optical Mouse Sensor Camera
This project demonstrates how to repurpose a legacy ADNS-2610 optical mouse sensor into a low-resolution ($18 \times 18$ pixel) digital camera. Using an Arduino UNO to interface with the sensor's serial registers and Python for real-time visualization, we can "see" the microscopic world the mouse uses for navigation.

How to Recreate This Project
1. Software Prerequisites
Before running the visualizer, ensure you have Python 3.x installed along with the following libraries:
     "pip install pyserial numpy matplotlib"

2. Hardware Requirements
Microcontroller: Arduino UNO (or any ATmega328P-based board).
Sensor: ADNS-2610 (found in most legacy optical mice).
Tools: Soldering iron, fine-tipped tweezers, and a craft knife (for trace cutting).

3. PCB Modification (The "Trace Cut" Procedure)
  To use the mouse's original PCB as a breakout board, you must isolate the sensor's communication lines from the original onboard controller to avoid signal contention.
    a.Identify the Pins: Locate the SDIO (Pin 3) and SCK (Pin 4) on the ADNS-2610 sensor.
    b.Cut the Traces: Using a craft knife, carefully cut the copper traces leading from these pins to the mouse’s original IC.
      Tip: Use a multimeter in continuity mode to verify that the pins are no longer electrically connected to the original controller.
    c.Preserve the Clock: Do not cut the traces connected to the 24 MHz resonator or the power/ground planes. The sensor needs the original resonator to function correctly.
    d.Solder Jumpers: Solder thin jumper wires directly to the isolated SDIO and SCK pads on the PCB.

4. Wiring Diagram
Connect the jumper wires to your Arduino as follows:

ADNS-2610 Pin |Function      |Arduino Pin
-----------------------------------------------
Pin 3         |SDIO (Data)   |Digital Pin 2
Pin 4         |SCK (Clock)   |Digital Pin 3
Pin 7         |GND           |GND
Pin 8         |VDD (5V)      |5V

5. Execution
Flash the Firmware: Open "mouse_camera.ino" in the Arduino IDE and upload it to your board.

Launch the Visualizer: Run the Python script:
          "pixel_visualizer.py"
