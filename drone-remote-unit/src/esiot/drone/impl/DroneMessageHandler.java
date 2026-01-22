package esiot.drone.impl;

import esiot.drone.api.MessageHandler;
import esiot.drone.gui.DroneControlGUI;
import esiot.drone.gui.DroneState;
import esiot.drone.gui.HangarState;

/**
 * Implementation of ArduinoMessageHandler that updates the DroneControlGUI
 * based on messages received from Arduino.
 */
public class DroneMessageHandler implements MessageHandler {

    private final DroneControlGUI gui;

    // Messages from Arduino
    private static final String MSG_DRONE_INSIDE = "DRONE INSIDE";
    private static final String MSG_TAKING_OFF = "TAKE OFF";
    private static final String MSG_DRONE_OUT = "DRONE OUT";
    private static final String MSG_LANDING = "LANDING";
    private static final String MSG_ALARM = "ALARM";
    private static final String MSG_PRE_ALARM = "PRE ALARM";
    private static final String MSG_NORMAL = "NORMAL";
    
    private static final String MSG_DISTANCE_PREFIX = "DIST:";
    private static final String MSG_TEMP_PREFIX = "TEMP:";

    public DroneMessageHandler(DroneControlGUI gui) {
        this.gui = gui;
    }

    @Override
    public void handleMessage(String msg) {
        // Check for distance updates (format: "DIST:XX.XX")
        if (msg.startsWith(MSG_DISTANCE_PREFIX)) {
            handleDistanceMessage(msg);
            return;
        }
        // Check for temperature updates (format: "TEMP:XX.XX")
        if (msg.startsWith(MSG_TEMP_PREFIX)) {
            handleTemperatureMessage(msg);
            return;
        }
        gui.log("Arduino: " + msg);
        handleStateMessage(msg.trim().toUpperCase());
    }

    /**
     * Handle distance messages (format: "DIST:XX.XX")
     */
    private void handleDistanceMessage(String msg) {
        try {
            double distance = Double.parseDouble(msg.substring(MSG_DISTANCE_PREFIX.length()).trim());
            gui.updateDistance(distance);
        } catch (NumberFormatException e) {
            gui.log("Invalid distance format: " + msg);
        }
    }

    /**
     * Handle temperature messages (format: "TEMP:XX.XX")
     */
    private void handleTemperatureMessage(String msg) {
        try {
            double temp = Double.parseDouble(msg.substring(MSG_TEMP_PREFIX.length()).trim());
            gui.updateTemperature(temp);
        } catch (NumberFormatException e) {
            gui.log("Invalid temperature format: " + msg);
        }
    }

    /**
     * Handle state change messages from Arduino
     */
    private void handleStateMessage(String msg) {
        switch (msg) {
            case MSG_DRONE_INSIDE:
                gui.updateDroneState(DroneState.REST);
                break;
                
            case MSG_TAKING_OFF:
                gui.updateDroneState(DroneState.TAKING_OFF);
                break;
                
            case MSG_DRONE_OUT:
                gui.updateDroneState(DroneState.OPERATING);
                break;
                
            case MSG_LANDING:
                gui.updateDroneState(DroneState.LANDING);
                break;
                
            case MSG_ALARM:
                gui.updateHangarState(HangarState.ALARM);
                break;
                
            case MSG_PRE_ALARM:
                gui.updateHangarState(HangarState.PRE_ALARM);
                break;
                
            case MSG_NORMAL:
                gui.updateHangarState(HangarState.NORMAL);
                break;
                
            default:
                break;
        }
    }
}
