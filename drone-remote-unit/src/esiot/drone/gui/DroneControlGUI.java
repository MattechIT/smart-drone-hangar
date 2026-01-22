package esiot.drone.gui;

import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

/**
 * GUI for Drone Remote Unit (DRU)
 * Controls for drone takeoff/landing and visualization of system state.
 */
public class DroneControlGUI extends JFrame {

    // GUI Components
    private JButton takeoffButton;
    private JButton landButton;
    private JLabel droneStateLabel;
    private JLabel droneStateIndicator;
    private JLabel hangarStateIndicator;
    private JLabel temperatureValueLabel;
    private JLabel distanceLabel;
    private JLabel distanceValueLabel;
    private JProgressBar distanceBar;
    private JTextArea logArea;
    private JLabel connectionStatusLabel;

    // Current states
    private DroneState currentDroneState = DroneState.REST;
    private HangarState currentHangarState = HangarState.NORMAL;
    private double currentDistance = 0.0;
    private boolean isConnected = false;
    private static final int MAX_DISTANCE = 100; // cm

    public DroneControlGUI() {
        initializeGUI();
    }

    private void initializeGUI() {
        setTitle("Drone Remote Unit (DRU) - Control Panel");
        setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        setSize(600, 550);
        setMinimumSize(new Dimension(500, 450));
        setLocationRelativeTo(null);

        // Main panel with padding
        JPanel mainPanel = new JPanel(new BorderLayout(10, 10));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // Top panel: Connection status
        JPanel topPanel = createConnectionPanel();
        mainPanel.add(topPanel, BorderLayout.NORTH);

        // Center panel: Status displays
        JPanel centerPanel = new JPanel(new GridLayout(1, 2, 10, 0));
        centerPanel.add(createDroneStatusPanel());
        centerPanel.add(createHangarStatusPanel());
        mainPanel.add(centerPanel, BorderLayout.CENTER);

        // Bottom panel: Controls and Log
        JPanel bottomPanel = new JPanel(new BorderLayout(10, 10));
        bottomPanel.add(createControlPanel(), BorderLayout.NORTH);
        bottomPanel.add(createLogPanel(), BorderLayout.CENTER);
        mainPanel.add(bottomPanel, BorderLayout.SOUTH);

        add(mainPanel);

        // Window close handler
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                int result = JOptionPane.showConfirmDialog(
                    DroneControlGUI.this,
                    "Are you sure you want to exit?",
                    "Confirm Exit",
                    JOptionPane.YES_NO_OPTION
                );
                if (result == JOptionPane.YES_OPTION) {
                    dispose();
                    System.exit(0);
                }
            }
        });

        // Initial state update
        updateDroneState(DroneState.REST);
        updateHangarState(HangarState.NORMAL);
        updateDistance(0);
    }

    private JPanel createConnectionPanel() {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        panel.setBorder(BorderFactory.createEtchedBorder());
        
        JLabel label = new JLabel("Connection: ");
        connectionStatusLabel = new JLabel("● Disconnected");
        connectionStatusLabel.setForeground(Color.RED);
        
        panel.add(label);
        panel.add(connectionStatusLabel);
        
        return panel;
    }

    private JPanel createDroneStatusPanel() {
        JPanel panel = new JPanel(new BorderLayout(5, 5));
        panel.setBorder(BorderFactory.createTitledBorder(
            BorderFactory.createEtchedBorder(),
            "Drone Status",
            TitledBorder.CENTER,
            TitledBorder.TOP,
            new Font("Arial", Font.BOLD, 14)
        ));

        // State indicator panel - use BoxLayout for vertical stacking from top
        JPanel statePanel = new JPanel();
        statePanel.setLayout(new BoxLayout(statePanel, BoxLayout.Y_AXIS));
        statePanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // State label
        JPanel stateLabelPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        droneStateLabel = new JLabel("State:");
        droneStateLabel.setFont(new Font("Arial", Font.PLAIN, 12));
        stateLabelPanel.add(droneStateLabel);
        stateLabelPanel.setMaximumSize(new Dimension(Integer.MAX_VALUE, 30));

        // State indicator (large colored box)
        droneStateIndicator = new JLabel("At Rest", SwingConstants.CENTER);
        droneStateIndicator.setOpaque(true);
        droneStateIndicator.setBackground(DroneState.REST.getColor());
        droneStateIndicator.setForeground(Color.WHITE);
        droneStateIndicator.setFont(new Font("Arial", Font.BOLD, 16));
        droneStateIndicator.setBorder(BorderFactory.createRaisedBevelBorder());
        droneStateIndicator.setPreferredSize(new Dimension(150, 50));
        droneStateIndicator.setMaximumSize(new Dimension(Integer.MAX_VALUE, 50));
        droneStateIndicator.setAlignmentX(Component.CENTER_ALIGNMENT);

        // Distance panel (shown during landing)
        JPanel distancePanel = new JPanel(new BorderLayout(5, 5));
        distancePanel.setBorder(BorderFactory.createTitledBorder("Distance to Ground"));
        distancePanel.setMaximumSize(new Dimension(Integer.MAX_VALUE, 80));
        
        distanceLabel = new JLabel("Distance:");
        distanceValueLabel = new JLabel("-- cm");
        distanceValueLabel.setFont(new Font("Arial", Font.BOLD, 14));
        
        distanceBar = new JProgressBar(0, MAX_DISTANCE);
        distanceBar.setStringPainted(true);
        distanceBar.setString("-- cm");
        
        JPanel distanceInfoPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        distanceInfoPanel.add(distanceLabel);
        distanceInfoPanel.add(distanceValueLabel);
        
        distancePanel.add(distanceInfoPanel, BorderLayout.NORTH);
        distancePanel.add(distanceBar, BorderLayout.CENTER);

        statePanel.add(stateLabelPanel);
        statePanel.add(droneStateIndicator);
        statePanel.add(Box.createVerticalStrut(10)); // Spacing
        statePanel.add(distancePanel);
        statePanel.add(Box.createVerticalGlue()); // Push everything to top

        panel.add(statePanel, BorderLayout.CENTER);

        return panel;
    }

    private JPanel createHangarStatusPanel() {
        JPanel panel = new JPanel(new BorderLayout(5, 5));
        panel.setBorder(BorderFactory.createTitledBorder(
            BorderFactory.createEtchedBorder(),
            "Hangar Status",
            TitledBorder.CENTER,
            TitledBorder.TOP,
            new Font("Arial", Font.BOLD, 14)
        ));

        // Use BoxLayout for vertical stacking
        JPanel contentPanel = new JPanel();
        contentPanel.setLayout(new BoxLayout(contentPanel, BoxLayout.Y_AXIS));
        contentPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // State indicator
        hangarStateIndicator = new JLabel("Normal", SwingConstants.CENTER);
        hangarStateIndicator.setOpaque(true);
        hangarStateIndicator.setBackground(HangarState.NORMAL.getColor());
        hangarStateIndicator.setForeground(Color.WHITE);
        hangarStateIndicator.setFont(new Font("Arial", Font.BOLD, 18));
        hangarStateIndicator.setBorder(BorderFactory.createRaisedBevelBorder());
        hangarStateIndicator.setMaximumSize(new Dimension(Integer.MAX_VALUE, 60));
        hangarStateIndicator.setAlignmentX(Component.CENTER_ALIGNMENT);

        // Temperature panel
        JPanel temperaturePanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        temperaturePanel.setBorder(BorderFactory.createTitledBorder("Temperature"));
        temperaturePanel.setMaximumSize(new Dimension(Integer.MAX_VALUE, 60));
        
        temperatureValueLabel = new JLabel("-- °C");
        temperatureValueLabel.setFont(new Font("Arial", Font.BOLD, 16));
        temperaturePanel.add(temperatureValueLabel);
        temperaturePanel.setAlignmentX(Component.CENTER_ALIGNMENT);

        contentPanel.add(hangarStateIndicator);
        contentPanel.add(Box.createVerticalStrut(10)); // Spacing
        contentPanel.add(temperaturePanel);
        contentPanel.add(Box.createVerticalGlue()); // Push to top

        panel.add(contentPanel, BorderLayout.CENTER);

        return panel;
    }

    private JPanel createControlPanel() {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.CENTER, 20, 10));
        panel.setBorder(BorderFactory.createTitledBorder(
            BorderFactory.createEtchedBorder(),
            "Drone Commands",
            TitledBorder.CENTER,
            TitledBorder.TOP,
            new Font("Arial", Font.BOLD, 14)
        ));

        // Takeoff button
        takeoffButton = new JButton("TAKEOFF");
        takeoffButton.setFont(new Font("Arial", Font.BOLD, 14));
        takeoffButton.setPreferredSize(new Dimension(150, 50));
        takeoffButton.setBackground(new Color(70, 130, 180));
        takeoffButton.setForeground(Color.BLACK);
        takeoffButton.setFocusPainted(false);
        takeoffButton.setToolTipText("Request drone takeoff from hangar");

        // Land button
        landButton = new JButton("LAND");
        landButton.setFont(new Font("Arial", Font.BOLD, 14));
        landButton.setPreferredSize(new Dimension(150, 50));
        landButton.setBackground(new Color(34, 139, 34));
        landButton.setForeground(Color.BLACK);
        landButton.setFocusPainted(false);
        landButton.setToolTipText("Request drone landing in hangar");

        panel.add(takeoffButton);
        panel.add(landButton);

        return panel;
    }

    private JPanel createLogPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createTitledBorder(
            BorderFactory.createEtchedBorder(),
            "System Log",
            TitledBorder.LEFT,
            TitledBorder.TOP
        ));

        logArea = new JTextArea(6, 40);
        logArea.setEditable(false);
        logArea.setFont(new Font("Monospaced", Font.PLAIN, 11));
        logArea.setBackground(new Color(40, 40, 40));
        logArea.setForeground(new Color(0, 255, 0));
        
        JScrollPane scrollPane = new JScrollPane(logArea);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

        // Clear log button
        JButton clearLogButton = new JButton("Clear Log");
        clearLogButton.addActionListener(e -> logArea.setText(""));

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        buttonPanel.add(clearLogButton);

        panel.add(scrollPane, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);

        return panel;
    }

    // === Public Methods for State Updates ===

    /**
     * Update the drone state display
     */
    public void updateDroneState(DroneState state) {
        SwingUtilities.invokeLater(() -> {
            currentDroneState = state;
            droneStateIndicator.setText(state.getDisplayName());
            droneStateIndicator.setBackground(state.getColor());
            
            // Enable/disable buttons based on state
            updateButtonStates();
            
            // Reset distance display when not in landing mode
            if (state != DroneState.LANDING || state != DroneState.TAKING_OFF) {
                distanceValueLabel.setText("-- cm");
                distanceBar.setValue(0);
                distanceBar.setString("-- cm");
            }
            
            log("Drone state changed to: " + state.getDisplayName());
        });
    }

    /**
     * Update the hangar state display
     */
    public void updateHangarState(HangarState state) {
        SwingUtilities.invokeLater(() -> {
            currentHangarState = state;
            hangarStateIndicator.setText(state.getDisplayName());
            hangarStateIndicator.setBackground(state.getColor());
            
            // Flash effect for alarm
            if (state == HangarState.ALARM) {
                hangarStateIndicator.setFont(new Font("Arial", Font.BOLD, 20));
            } else {
                hangarStateIndicator.setFont(new Font("Arial", Font.BOLD, 18));
            }
            
            updateButtonStates();
            log("Hangar state changed to: " + state.getDisplayName());
        });
    }

    /**
     * Update the temperature display
     */
    public void updateTemperature(double tempC) {
        SwingUtilities.invokeLater(() -> {
            String tempStr = String.format("%.2f °C", tempC);
            temperatureValueLabel.setText(tempStr);
        });
    }

    /**
     * Update the distance display (during landing)
     */
    public void updateDistance(double distanceCm) {
        SwingUtilities.invokeLater(() -> {
            currentDistance = distanceCm;
            String distanceStr = String.format("%.2f cm", distanceCm);
            distanceValueLabel.setText(distanceStr);
            distanceBar.setValue((int)Math.min(distanceCm, MAX_DISTANCE));
            distanceBar.setString(distanceStr);
            // Color based on distance
            if (distanceCm < 10) {
                distanceBar.setForeground(new Color(50, 205, 50)); // Green - close
            } else if (distanceCm < 30) {
                distanceBar.setForeground(new Color(255, 165, 0)); // Orange - medium
            } else {
                distanceBar.setForeground(new Color(70, 130, 180)); // Blue - far
            }
        });
    }

    /**
     * Update connection status display
     */
    public void setConnected(boolean connected) {
        SwingUtilities.invokeLater(() -> {
            isConnected = connected;
            if (connected) {
                connectionStatusLabel.setText("● Connected");
                connectionStatusLabel.setForeground(new Color(50, 205, 50));
            } else {
                connectionStatusLabel.setText("● Disconnected");
                connectionStatusLabel.setForeground(Color.RED);
            }
            updateButtonStates();
        });
    }

    /**
     * Add a message to the log
     */
    public void log(String message) {
        SwingUtilities.invokeLater(() -> {
            String timestamp = java.time.LocalTime.now().format(
                java.time.format.DateTimeFormatter.ofPattern("HH:mm:ss")
            );
            logArea.append("[" + timestamp + "] " + message + "\n");
            logArea.setCaretPosition(logArea.getDocument().getLength());
        });
    }

    /**
     * Update button enabled states based on current system state
     */
    private void updateButtonStates() {
        boolean canTakeoff = isConnected 
                          && (currentDroneState == DroneState.REST) 
                          && (currentHangarState != HangarState.ALARM) && (currentHangarState != HangarState.PRE_ALARM);
        boolean canLand = isConnected 
                       && (currentDroneState == DroneState.OPERATING) 
                       && (currentHangarState != HangarState.ALARM) && (currentHangarState != HangarState.PRE_ALARM);

        takeoffButton.setEnabled(canTakeoff);
        landButton.setEnabled(canLand);

        // Visual feedback for disabled buttons
        takeoffButton.setBackground(canTakeoff ? new Color(70, 130, 180) : Color.GRAY);
        landButton.setBackground(canLand ? new Color(34, 139, 34) : Color.GRAY);
    }

    // === Event Listener Registration ===

    /**
     * Set the action listener for the takeoff button
     */
    public void setTakeoffButtonListener(ActionListener listener) {
        takeoffButton.addActionListener(listener);
    }

    /**
     * Set the action listener for the land button
     */
    public void setLandButtonListener(ActionListener listener) {
        landButton.addActionListener(listener);
    }

    // === Getters ===

    public DroneState getCurrentDroneState() {
        return currentDroneState;
    }

    public HangarState getCurrentHangarState() {
        return currentHangarState;
    }

    public double getCurrentDistance() {
        return currentDistance;
    }

    // === Main method for standalone testing ===
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            } catch (Exception e) {
                e.printStackTrace();
            }
            
            DroneControlGUI gui = new DroneControlGUI();
            gui.setVisible(true);
            
            // Demo: simulate state changes
            gui.setConnected(true);
            gui.log("System initialized");
            gui.log("Ready for commands");
            
            // Add demo listeners
            gui.setTakeoffButtonListener(e -> {
                gui.log("Takeoff command sent");
                gui.updateDroneState(DroneState.TAKING_OFF);
            });
            
            gui.setLandButtonListener(e -> {
                gui.log("Land command sent");
                gui.updateDroneState(DroneState.LANDING);
                gui.updateDistance(50);
            });
        });
    }
}
