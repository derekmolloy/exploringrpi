#ifndef ADXL345_H_
#define ADXL345_H_
#include "SPIDevice.h"

/// The ADXL345 has 0x40 registers (0x01 to 0x1C are reserved and should not be accessed)
#define BUFFER_SIZE 0x40

namespace exploringRPi {
/**
 * @class ADXL345
 * @brief Specific class for the ADXL345 Accelerometer
 */
class ADXL345{
public:
   enum RANGE {            //!< Define g range of the sensor
      PLUSMINUS_2_G  = 0,  //!< plus/minus 2g
      PLUSMINUS_4_G  = 1,  //!< plus/minus 4g
      PLUSMINUS_8_G  = 2,  //!< plus/minus 8g
      PLUSMINUS_16_G = 3   //!< plus/minus 16g
   };
   enum RESOLUTION {       //!< Resolution high in +/-16g only
      NORMAL = 0,          //!< NORMAL 10-bit resolution
      HIGH = 1             //!< HIGH 13-bit resolution
   };
private:
   SPIDevice *device;
   unsigned char *registers;
   ADXL345::RANGE range;
   ADXL345::RESOLUTION resolution;
   short accelerationX, accelerationY, accelerationZ; // raw 2's complement
   float pitch, roll;                                 // in degrees
   short combineRegisters(unsigned char msb, unsigned char lsb);
   void calculatePitchAndRoll();
   int updateRegisters();
public:
   ADXL345(SPIDevice *busDevice);
   virtual int readSensorState();
   virtual void setRange(ADXL345::RANGE range);
   virtual ADXL345::RANGE getRange() { return this->range; }
   virtual void setResolution(ADXL345::RESOLUTION resolution);
   virtual ADXL345::RESOLUTION getResolution() { return this->resolution; }
   virtual short getAccelerationX() { return accelerationX; }
   virtual short getAccelerationY() { return accelerationY; }
   virtual short getAccelerationZ() { return accelerationZ; }
   virtual float getPitch() { return pitch; }
   virtual float getRoll() { return roll; }
   virtual void displayPitchAndRoll(int iterations = 600);
   virtual ~ADXL345();
};

} /* namespace exploringRPi */

#endif /* ADXL345_H_ */
