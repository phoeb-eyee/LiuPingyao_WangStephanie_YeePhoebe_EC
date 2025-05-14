import com.fazecast.jSerialComm.SerialPort;

import java.util.Scanner;
import java.time.LocalTime;

public class SGP40Reader {
    public static void main(String[] args) throws Exception {
        // Get all available ports
        SerialPort[] ports = SerialPort.getCommPorts();

        // get the serial port for Arduino
        SerialPort arduinoPort = null;
        for (SerialPort port: ports) {
            if (port.getSystemPortName().contains("cu.usbmodem")) {
                System.out.println("Port cu.usbmodem found.");
                arduinoPort = port;
                break;
            }6
        }

        if (arduinoPort == null) {
            throw new Exception("Arduino Port Not Found.");
        }

        arduinoPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 1000, 0);

        arduinoPort.setBaudRate(9600);
        if (!arduinoPort.openPort()) {
            System.out.println("Failed to open port.");
            return;
        }

        System.out.println("Reading from Arduino (SGP40)...");
        int prevValue = -1;
        while (true)
        {
            LocalTime time = LocalTime.now();
            Scanner data = new Scanner(arduinoPort.getInputStream());
            while (data.hasNextLine()) {
                try {
                    String line = data.nextLine();
                    int value = Integer.parseInt(line);
                    String result = "UNKNOWN";
                    if (value >= 500) {
                        result = "VERY HIGH VOC - UNHEALTHY";
                    } else if (value >= 300) {
                        result = "HIGH VOC - INDOOR AIR POLLUTION DETECTED";
                    } else if (value >= 100) {
                        result = "MODERATE VOC - TYPICAL INDOOR AIR";
                    } else if (value > 0) {
                        result = "LOW VOC - CLEAN AIR";
                    } else {
                        result = "ERROR IN DETECTION";
                    }
                    if (value > 0 && value != prevValue) {
                        System.out.println("[" + time + "] VOC Index: " + value + " " + result);
                        prevValue = value;
                    }
                } catch (Exception e) {
                    System.out.println("[" + time + "] Error reading: " + e.getMessage());
                }
            }

        }
    }
}
