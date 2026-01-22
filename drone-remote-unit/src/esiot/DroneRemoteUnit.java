package esiot;

import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import esiot.drone.gui.DroneControlGUI;
import esiot.drone.gui.DroneState;
import esiot.drone.impl.DRUController;
import esiot.serial.SerialCommChannel;

/**
 * Main entry point for the Drone Remote Unit with GUI interface.
 * This replaces the console-based DroneRemoteUnit for a graphical interface.
 */
public class DroneRemoteUnit {
    
    private static final String SERIAL_PORT = "COM6";
    private static final int BAUD_RATE = 115200;
    private static final int ARDUINO_BOOT_DELAY = 4000;

    public static void main(String[] args) {
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception e) {
            e.printStackTrace();
        }

        SwingUtilities.invokeLater(() -> {
            DroneControlGUI gui = new DroneControlGUI();
            gui.setVisible(true);
            gui.updateDroneState(DroneState.REST);
            gui.log("Initializing system...");
            
            // Connect to Arduino in background thread
            new Thread(() -> {
                try {
                    gui.log("Opening serial port " + SERIAL_PORT + "...");
                    SerialCommChannel channel = new SerialCommChannel(SERIAL_PORT, BAUD_RATE);
                    gui.log("Waiting for Arduino to reboot (" + (ARDUINO_BOOT_DELAY/1000) + "s)...");
                    Thread.sleep(ARDUINO_BOOT_DELAY);
                    gui.log("Creating controller...");
                    DRUController controller = new DRUController(gui, channel);
                    controller.start();
                    gui.log("System ready!");
                    gui.log("Use TAKEOFF/LAND buttons to control the drone.");
                    
                    Runtime.getRuntime().addShutdownHook(new Thread(() -> {
                        gui.log("Shutting down...");
                        controller.stop();
                        channel.close();
                    }));
                    
                } catch (Exception e) {
                    gui.log("ERROR: " + e.getMessage());
                    gui.log("Check that Arduino is connected to " + SERIAL_PORT);
                    e.printStackTrace();
                }
            }, "Serial-Init").start();
        });
    }
}
