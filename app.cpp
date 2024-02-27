#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "I2CDevice.h"

namespace EE513 {

// Derived class for the RTC module
class RTCModule : public I2CDevice {
public:
    // Constructor inheriting from the base class constructor
    RTCModule(unsigned int bus, unsigned int device) : I2CDevice(bus, device) {}

    // Function to read and display the current time from the RTC
    void readAndDisplayTime() {
        // Read the current time registers from the RTC (assuming RTC registers are known)
        unsigned char* timeRegisters = readRegisters(7); // Assuming 7 registers for hours, minutes, seconds, etc.

        // Display the current time
        std::cout << "Current Time: ";
        std::cout << std::setfill('0') << std::setw(2) << static_cast<int>(timeRegisters[2]) << ":"; // hours
        std::cout << std::setfill('0') << std::setw(2) << static_cast<int>(timeRegisters[1]) << ":"; // minutes
        std::cout << std::setfill('0') << std::setw(2) << static_cast<int>(timeRegisters[0]);         // seconds
        std::cout << std::endl;

        // Clean up dynamically allocated memory
        delete[] timeRegisters;
    }
};

} // namespace EE513

int main() {
    // Create an instance of the RTCModule class
    EE513::RTCModule rtc(1, 0x68); // Example bus number and device address (modify as per your setup)

    // Open the I2C device
    rtc.open();

    // Read and display the current time from the RTC
    rtc.readAndDisplayTime();

    // Close the I2C device
    rtc.close();

    return 0;
}

