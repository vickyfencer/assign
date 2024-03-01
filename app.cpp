#include <iostream>
#include "I2CDevice.h"
#include <bitset>
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
// Function to enable square-wave output at 1Hz on the RTC module
    void enableSquareWaveOutput() {
        // Set SQWEN (Square Wave Output Enable) bit (bit 4) and clear RS2 and RS1 bits to set 1Hz frequency
        writeRegister(0x0E, 0x03); // Control register address is 0x0E
    }
  // Set Alarm to trigger at every second
    void setAlarmEverySecond() {
unsigned int a,b,c,d ;
std::cout<< "BeforeContReg"<< std::bitset<8> (readRegister(0x0E))<<std::endl;
       
writeRegister(0x0E, 0b00011101);  // Set Alarm 1 registers to trigger every second (A1M4-A1M1: 1111)
 a =readRegister(0x07); 
a|= 0b10000000; // A1M1-A1M4: 1111 (Every second)
writeRegister(0x07,a);
 b =readRegister(0x08);
b|= 0b10000000; // A1M1-A1M4: 1111 (Every second)
writeRegister(0x08,b);
 c =readRegister(0x09);
c|= 0b10000000; // A1M1-A1M4: 1111 (Every second)
writeRegister(0x09,c);
 d =readRegister(0x0A);
d|= 0b10000000; // A1M1-A1M4: 1111 (Every second)
writeRegister(0x0A,d);
std::cout<< "ContReg"<< std::bitset<8> (readRegister(0x0E))<<std::endl;

std::cout<< "1AM"<< std::bitset<8> (readRegister(0x07))<<std::endl;
std::cout<< "2AM"<< std::bitset<8> (readRegister(0x08))<<std::endl;
std::cout<< "3AM"<< std::bitset<8> (readRegister(0x09))<<std::endl;
std::cout<< "4AM"<< std::bitset<8> (readRegister(0x0A))<<std::endl;


    }

 void clearInterruptFlag() {
        unsigned char statusReg = readRegister(0x0F); // Read the status register
        statusReg &= ~(0x00000001); // Clear A1F (Alarm 1 Flag) bit
        writeRegister(0x0F, statusReg); // Write back to the status register
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
rtc.debugDumpRegisters(14);
   rtc.setAlarmEverySecond();

rtc.debugDumpRegisters(14);
 rtc.clearInterruptFlag();
rtc.debugDumpRegisters(14);
  //rtc.enableSquareWaveOutput();

    return 0;
}

