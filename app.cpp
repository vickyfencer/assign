#include <iostream>
#include "I2CDevice.h"

namespace EE513 {

class DS3231 : public I2CDevice {
public:
    DS3231(unsigned int bus, unsigned int device) : I2CDevice(bus, device) {}

    // Read and display the current RTC module time and date
    void readAndDisplayTimeDate() {
        unsigned char *timeDate = readRegisters(7, 0x00); // Read time and date registers starting from address 0x00
        // Parse time and date values
        int seconds = bcdToDec(timeDate[0]& 0x7F); // Masking to ignore CH bit
        int minutes = bcdToDec(timeDate[1]& 0X7F);
        int hours = bcdToDec(timeDate[2] & 0x3F); // Masking to ignore 12/24 hour bit
        int day = bcdToDec(timeDate[3]) & 0X07;
        int date = bcdToDec(timeDate[4])& 0X3F;
        int month = bcdToDec(timeDate[5] & 0x1F); // Masking to ignore century bit
        int year = bcdToDec(timeDate[6]);
        // Display time and date
        std::cout << "Time: " << hours << ":" << minutes << ":" << seconds << std::endl;
        std::cout << "Date: " << year << "-" << month << "-" << date << " Day: " << day << std::endl;
    }

    // Read and display the current temperature
    void readAndDisplayTemperature() {
        unsigned char tempMSB = readRegister(0x11);
        unsigned char tempLSB = readRegister(0x12);
        int tempInteger = (int)tempMSB;
        if (tempMSB & 0x80) { // Check if temperature is negative
            tempInteger -= 256;
        }
        float tempFraction = (float)(tempLSB >> 6) * 0.25;
        float temperature = tempInteger + tempFraction;
        std::cout << "Temperature: " << temperature << "°C" << std::endl;
    }

    void setTimeDate(int year, int month, int date, int day, int hours, int minutes, int seconds) {
    // Convert decimal values to BCD
    unsigned char timeDate[7];
    timeDate[0] = decToBcd(seconds);
    timeDate[1] = decToBcd(minutes);
    timeDate[2] = decToBcd(hours);
    timeDate[3] = decToBcd(day);
    timeDate[4] = decToBcd(date);
    timeDate[5] = decToBcd(month);
    timeDate[6] = decToBcd(year);

for (int i = 0; i < 7; ++i) {
        writeRegister(i, timeDate[i]);
    } 
}


private:
    // Helper function to convert binary coded decimal (BCD) to decimal
    int bcdToDec(unsigned char bcd) {
        return ((bcd >> 4) * 10) + (bcd & 0x0F);
    }

    // Helper function to convert decimal to binary coded decimal (BCD)
    unsigned char decToBcd(int dec) {
        return ((dec / 10) << 4) | (dec % 10);
    }
};

} /* namespace EE513 */

int main() {
    // Initialize DS3231 object with the appropriate bus and device numbers
    EE513::DS3231 rtc(1, 0x68);

    // Read and display the current time and date
    std::cout << "Current Time and Date:" << std::endl;
    rtc.readAndDisplayTimeDate();

    // Read and display the current temperature
    std::cout << "Current Temperature:" << std::endl;
    rtc.readAndDisplayTemperature();

    // Set a new time and date (e.g., February 28, 2024, 15:30:00, Wednesday)
   rtc.setTimeDate(2024, 2, 28, 3, 15, 30, 0);

    // Verify the new time and date has been set
 std::cout << "New Time and Date:" << std::endl;
  rtc.readAndDisplayTimeDate();

    return 0;
}

