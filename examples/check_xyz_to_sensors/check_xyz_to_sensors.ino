#include <legControl.h>

legControl leg1;

        float startingAngles[3] = {0, 80, 100};
        float goalAngles[3] = {30, 60, 20};
        float startingXYZ[3];
        float goalXYZ[3];
        float sensorGoals[3];

       float sensors[3] = {1724.00, 255.00, 1273.00};
        float xyz[3] = {50, 0, -3};


void setup() {
        Serial.begin(9600); 
        while(!Serial);

        leg1.anglesToXYZ(startingAngles, startingXYZ);
        leg1.anglesToXYZ(goalAngles, goalXYZ);
        
        //int startingSensors[3];
        //int goalSensors[3];
        float sensorVelocities[3];
        float move_time = 10; //in seconds

        //leg1.goal_XYZ_toSensorVelocities(startingXYZ, goalXYZ, sensorVelocities, move_time);
        // Serial.println("staring");
        // for (int i = 0; i < 3; i++) {
        //         Serial.print("angle:joint ");
        //         Serial.print(startingAngles[i]);
        //         Serial.print(" : ");
        //         Serial.print(i);
        //         Serial.print('\t');
        //         Serial.print("sensor: ");
        //         Serial.println(leg1.angleToSensor(i, startingAngles[i]));
        // }
        // Serial.println("ending");
        // for (int i = 0; i < 3; i++) {
        //         Serial.print("angle:joint ");
        //         Serial.print(goalAngles[i]);
        //         Serial.print(" : ");
        //         Serial.print(i);
        //         Serial.print('\t');
        //         Serial.print("sensor: ");
        //         Serial.println(leg1.angleToSensor(i, goalAngles[i]));
        // }

        Serial.println("xyz in:");
        for (int i = 0; i < 3; i ++) {
                Serial.print(xyz[i]);
                Serial.print(" ");
        }
        Serial.println();

        leg1.xyzToSensors(xyz, sensors);
        Serial.println("sensors out:");

        for (int i = 0; i < 3; i ++) {
                Serial.print(sensors[i]);
                Serial.print(" ");
        }
        Serial.println();


        // Serial.println("staring sensors");
        // for (int i = 0; i < 3; i ++) {
        //         Serial.print(sensors[i]); 
        //         Serial.print(" ");
        // }
        // Serial.println();
        leg1.sensors_to_xyz(sensors, xyz);

        Serial.println("sensors back in to xyz");
        for (int i = 0; i < 3; i ++) {
                Serial.print(xyz[i]);
                Serial.print(" ");
        }
        Serial.println();

        Serial.println("xyz back again to sensors:");
        leg1.xyzToSensors(xyz, sensors);
        for (int i = 0; i < 3; i ++) {
                Serial.print(sensors[i]);
                Serial.print(" ");
        }
        Serial.println();

        Serial.println("finally, sensors back to xyz:");
        leg1.sensors_to_xyz(sensors, xyz);
        for (int i = 0; i < 3; i ++) {
                Serial.print(xyz[i]);
                Serial.print(" ");
        }
        Serial.println();

        

}

void loop() {

        // leg1.anglesToXYZ(startingAngles, startingXYZ);
        // leg1.anglesToXYZ(goalAngles, goalXYZ);
        
        // //int startingSensors[3];
        // //int goalSensors[3];
        // float sensorVelocities[3];
        // float move_time = 10; //in seconds

        // //leg1.goal_XYZ_toSensorVelocities(startingXYZ, goalXYZ, sensorVelocities, move_time);
        
        // for (int i = 0; i < 3; i++) {
        //         Serial.print("angle:joint ");
        //         Serial.print(startingAngles[i]);
        //         Serial.print(":");
        //         Serial.print(i);
        //         Serial.print('\t');
        //         Serial.print("sensor: ");
        //         Serial.println(leg1.angleToSensor(i, startingAngles[i]));
        // }

        
        

        // // Serial.print("sensor velocities: ");
        // // for (int i = 0; i < 3; i ++) {
        // //         Serial.print(sensorVelocities[i]);
        // //         Serial.print("\t");
        // // }
        // // Serial.println();
        
       

        delay(2000);
}