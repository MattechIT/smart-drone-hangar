package esiot.drone.gui;

import java.awt.Color;

// Drone states
public enum DroneState {
    REST("At Rest", new Color(100, 149, 237)),           // Cornflower Blue
    TAKING_OFF("Taking Off", new Color(255, 165, 0)),    // Orange
    OPERATING("Operating", new Color(50, 205, 50)),      // Lime Green
    LANDING("Landing", new Color(255, 215, 0));          // Gold

    private final String displayName;
    private final Color color;

    DroneState(String displayName, Color color) {
        this.displayName = displayName;
        this.color = color;
    }

    public String getDisplayName() { return displayName; }
    public Color getColor() { return color; }
}
