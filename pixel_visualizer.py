import serial
import numpy as np
import matplotlib.pyplot as plt


SERIAL_PORT = 'COM11'  # Arduino's port
BAUD_RATE = 115200
GRID_SIZE = 18        # 18x18 pixels

def main():
    try:
        # Initialize Serial Connection
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to {SERIAL_PORT}. Close image window to exit.")
    except Exception as e:
        print(f"Error: Could not open port {SERIAL_PORT}. {e}")
        return

    # Setup the live plot
    plt.ion() 
    fig, ax = plt.subplots(figsize=(6, 6))
    
    # Sensor data is 6-bit (0-63).set vmin/vmax to keep the contrast stable.
    # cmap='gray' gives us the black-and-white camera look.
    img_display = ax.imshow(np.zeros((GRID_SIZE, GRID_SIZE)), 
                            cmap='gray', vmin=0, vmax=63, interpolation='nearest')
    
    plt.title("Mouse Camera Live Pixel Dump")
    plt.axis('off') 

    current_frame = []

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            
            # Check for the frame marker we set in the Arduino code
            if "---FRAME_END---" in line:
                if len(current_frame) == GRID_SIZE:
                    # Update the image display with the 18x18 array
                    img_display.set_data(np.array(current_frame))
                    plt.draw()
                    plt.pause(0.01) # Short pause to allow UI to update
                
                current_frame = [] # Clear for next frame
            else:
                
                try:
                    row = [int(val) for val in line.split(',') if val.strip().isdigit()]
                    if len(row) == GRID_SIZE:
                        current_frame.append(row)
                except ValueError:
                    continue

if __name__ == "__main__":
    main()