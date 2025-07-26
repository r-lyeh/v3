// Function to check if a double-click occurred for a given mouse button
int glfwGetMouseDoubleClick(int button, double timeout_ss) {
    // Get the current state of the mouse button
    int current_state = glfwGetMouseButton(glfwGetCurrentContext(), button);
    
    // Static arrays to store the last two press times and previous button state
    static double down0[3] = {0}; // Previous press time
    static double down1[3] = {0}; // Current press time
    static int previous[3] = {0}; // Previous button state

    // Check for a new press (transition from not pressed to pressed)
    if (current_state == GLFW_PRESS && !previous[button]) {
        // Update the press times
        down0[button] = down1[button];
        down1[button] = glfwGetTime();

        // Update the button state
        previous[button] = 1;

        // Return true if the time between presses is less than the threshold (ss)
        return (down1[button] - down0[button]) < timeout_ss;
    }

    // Update the button state if the button is released
    if (current_state == GLFW_RELEASE) {
        previous[button] = 0;
    }

    return 0;
}
