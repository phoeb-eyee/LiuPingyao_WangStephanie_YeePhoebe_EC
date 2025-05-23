import com.fazecast.jSerialComm.SerialPort;  
// Import the jSerialComm library to handle serial communication with devices.

import java.util.Scanner;
// Import Scanner to read input from the serial port.

import java.time.LocalTime;
// Import LocalTime to timestamp readings.

public class SGP40Reader {
    public static void main(String[] args) throws Exception {
        // Get all available serial ports on the system
        SerialPort[] ports = SerialPort.getCommPorts();

        // Find the serial port connected to the Arduino by matching the port name pattern
        SerialPort arduinoPort = null;
        for (SerialPort port: ports) {
            if (port.getSystemPortName().contains("cu.usbmodem")) {
                System.out.println("Port cu.usbmodem found.");
                arduinoPort = port;
                break;  // Stop searching once the port is found
            }
        }

        // If Arduino port was not found, throw an exception and exit
        if (arduinoPort == null) {
            throw new Exception("Arduino Port Not Found.");
        }

        // Set serial communication timeouts: semi-blocking read with a 1000 ms timeout
        arduinoPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 1000, 0);

        // Set the baud rate to 9600 to match Arduino serial speed
        arduinoPort.setBaudRate(9600);

        // Open the serial port connection
        if (!arduinoPort.openPort()) {
            System.out.println("Failed to open port.");
            return;  // Exit if unable to open port
        }

        System.out.println("Reading from Arduino (SGP40)...");

        int prevValue = -1;  // Store previous VOC value to avoid duplicate prints

        while (true) {
            LocalTime time = LocalTime.now();  // Get the current time for timestamping

            Scanner data = new Scanner(arduinoPort.getInputStream());  
            // Scanner reads lines from Arduino's serial output

            while (data.hasNextLine()) {
                try {
                    String line = data.nextLine();  // Read the next line of data
                    int value = Integer.parseInt(line);  // Parse the line as an integer VOC index

                    String result = "UNKNOWN";

                    // Map VOC index values to health risk messages
                    if (value >= 500) {
                        result = "VERY HIGH VOC - VERY HIGH SEIZURE RISK. Seek medical help if symptoms occur within the next 60 minutes (e.g., dizziness, nausea, light sensitivity).";
                    } else if (value >= 300) {
                        result = "HIGH VOC - HIGH SEIZURE RISK. Seek medical help if symptoms occur within the next 60 minutes.";
                    } else if (value >= 100) {
                        result = "MODERATE VOC - LOW SEIZURE RISK";
                    } else if (value > 0) {
                        result = "LOW VOC - NO SEIZURE RISK";
                    } else {
                        result = "ERROR IN DETECTION";
                    }

                    // Print VOC value and risk message only if it has changed and is valid
                    if (value > 0 && value != prevValue) {
                        System.out.println("[" + time + "] VOC Index: " + value + " " + result);
                        prevValue = value;
                    }
                } catch (Exception e) {
                    // Handle any parsing or IO errors gracefully
                    System.out.println("[" + time + "] Error reading: " + e.getMessage());
                }
            }
        }
    }
}
