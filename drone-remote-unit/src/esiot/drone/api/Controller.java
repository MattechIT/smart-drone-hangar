package esiot.drone.api;

/**
 * Interface for the Drone Remote Unit Controller.
 * Defines the contract for communication between GUI and the drone hangar system.
 */
public interface Controller {

    /**
     * Start the controller and begin listening for messages.
     */
    void start();

    /**
     * Stop the controller and release resources.
     */
    void stop();

    /**
     * Check if the controller is currently running.
     * @return true if running, false otherwise
     */
    boolean isRunning();

    /**
     * Send a takeoff request to the drone hangar.
     */
    void requestTakeoff();

    /**
     * Send a landing request to the drone hangar.
     */
    void requestLanding();
}
