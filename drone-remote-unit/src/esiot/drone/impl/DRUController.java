package esiot.drone.impl;

import esiot.drone.api.MessageHandler;
import esiot.drone.api.Controller;
import esiot.drone.gui.DroneControlGUI;
import esiot.serial.SerialCommChannel;

/**
 * Controller that bridges the DroneControlGUI with the SerialCommChannel.
 * Handles communication between the GUI and the Arduino-based Drone Hangar.
 */
public class DRUController implements Controller{

    private final DroneControlGUI gui;
    private final SerialCommChannel channel;
    private final MessageHandler messageHandler;
    private Thread listenerThread;
    private volatile boolean running = false;

    // Protocol messages for sending commands to Arduino
    private static final String CMD_TAKEOFF = "TAKE OFF";
    private static final String CMD_LANDING = "LANDING";

    public DRUController(DroneControlGUI gui, SerialCommChannel channel) {
        this.gui = gui;
        this.channel = channel;
        this.messageHandler = new DroneMessageHandler(gui);
        setupGUIListeners();
    }

    @Override
    public void start() {
        if (!running) {
            running = true;
            startMessageListener();
            gui.setConnected(true);
            gui.log("Controller started");
        }
    }

    @Override
    public void requestTakeoff() {
        gui.log("Sending TAKEOFF request...");
        channel.sendMsg(CMD_TAKEOFF);
    }

    @Override
    public void requestLanding() {
        gui.log("Sending LANDING request...");
        channel.sendMsg(CMD_LANDING);
    }

    /**
     * Setup action listeners for GUI buttons
     */
    private void setupGUIListeners() {
        gui.setTakeoffButtonListener(e -> requestTakeoff());

        gui.setLandButtonListener(e -> requestLanding());
    }

    /**
     * Start the background thread that listens for messages from Arduino
     */
    private void startMessageListener() {
        listenerThread = new Thread(() -> {
            while (running && !Thread.currentThread().isInterrupted()) {
                try {
                    if (channel.isMsgAvailable()) {
                        String msg = channel.receiveMsg();
                        messageHandler.handleMessage(msg);
                    }
                    Thread.sleep(50);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                } catch (Exception ex) {
                    gui.log("Error receiving message: " + ex.getMessage());
                    ex.printStackTrace();
                }
            }
        }, "Arduino-Listener");
        listenerThread.setDaemon(true);
        listenerThread.start();
    }

    /**
     * Stop the controller and release resources
     */
    @Override
    public void stop() {
        running = false;
        if (listenerThread != null) {
            listenerThread.interrupt();
            try {
                listenerThread.join(1000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        gui.setConnected(false);
        gui.log("Controller stopped");
    }

    /**
     * Check if the controller is running
     */
    @Override
    public boolean isRunning() {
        return running;
    }
}
