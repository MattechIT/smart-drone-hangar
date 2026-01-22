package esiot.drone.gui;

import java.awt.Color;

// Hangar states
public enum HangarState {
    NORMAL("Normal", new Color(50, 205, 50)),       // Green
    PRE_ALARM("Pre-Alarm", new Color(255, 165, 0)), // Orange
    ALARM("ALARM", new Color(255, 0, 0));           // Red

    private final String displayName;
    private final Color color;

    HangarState(String displayName, Color color) {
        this.displayName = displayName;
        this.color = color;
    }

    public String getDisplayName() { return displayName; }
    public Color getColor() { return color; }
}
