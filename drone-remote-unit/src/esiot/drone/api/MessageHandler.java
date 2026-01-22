package esiot.drone.api;

/**
 * Interface for handling messages received from Arduino.
 * Processes incoming messages and updates the system state accordingly.
 */
public interface MessageHandler {
    
    /**
     * Handle a message received from Arduino.
     * @param msg the message received
     */
    void handleMessage(String msg);
}
