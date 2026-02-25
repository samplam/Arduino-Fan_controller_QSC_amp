#include <OneWire.h>

OneWire temp_sensor_1(1); // Temperature sensor 1 on pin 1.
OneWire temp_sensor_2(2); // Temperature sensor 2 on pin 2.

int fan_pin = 0; // Fan on pin 0.
int threshold = 640; // Temperature at wich the fan starts (40 deg C x 16 = 640).
int temperature_max = 960; // Temperature at wich the fan is at maximum speed (60 deg C x 16 = 960).
int fan_speed = 0; // Speed of the fan.
int temperature_1 = 0;
int temperature_2 = 0;
int temperature = 0;
float factor = (212.5/(temperature_max-threshold)); /* Conversion factor for fan output. 212.5 comes from 4V for minimum speed on 24V. 
                                                    This ratio on 255 gives 42.5. So, 255-42.5 = 212.5 output for 4V on 24V supply. */

void setup() {
  pinMode (fan_pin, OUTPUT);
  analogWrite (fan_pin, 255);
  delay (4000);
  analogWrite (fan_pin, 0);
}

void loop() {
  byte i;
  byte data_1[12];
  byte addr_1[8];
  byte j;
  byte data_2[12];
  byte addr_2[8];

  if ( !temp_sensor_1.search(addr_1)) {
    temp_sensor_1.reset_search();
    delay(250);
    return;
  }

  if ( !temp_sensor_2.search(addr_2)) {
    temp_sensor_2.reset_search();
    delay(250);
    return;
  }

  temp_sensor_1.reset();
  temp_sensor_1.select(addr_1);
  temp_sensor_1.write(0x44, 0);

  temp_sensor_2.reset();
  temp_sensor_2.select(addr_2);
  temp_sensor_2.write(0x44, 0);

  delay(750);

  temp_sensor_1.reset();
  temp_sensor_1.select(addr_1);    
  temp_sensor_1.write(0xBE);
  
  temp_sensor_2.reset();
  temp_sensor_2.select(addr_2);    
  temp_sensor_2.write(0xBE);

  for ( i = 0; i < 9; i++) {
  data_1[i] = temp_sensor_1.read();
  }

  temperature_1=(data_1[1]*256)+data_1[0];

  for ( j = 0; j < 9; j++) {
  data_2[j] = temp_sensor_2.read();
  }

  temperature_2=(data_2[1]*256)+data_2[0];

  if (temperature_1 > temperature_2)
    {temperature = temperature_1;
    }
    else
    {temperature = temperature_2;
    }

  if (temperature >= threshold && temperature <= temperature_max)
    {fan_speed = int(((temperature-threshold)*factor)+42.5);
    }
    else
    {if (temperature > temperature_max)
      {fan_speed = 255;
      }
      else
      {fan_speed = 0;
      }
    }

   analogWrite (fan_pin, fan_speed);
   
}
