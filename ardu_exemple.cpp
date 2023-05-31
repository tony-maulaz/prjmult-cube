#include <AccelStepper.h>

const int rotation_90 = 0;
const int rotation_180 = 1;
const int rotation_270 = 2;

struct Command
{

    bool set_command(char id, char cmd, char arg, char crc)
    {
        //if (id == this->id && id + cmd + arg == crc)
        if (id == this->id)
        {
            this->cmd = cmd;
            this->arg1 = arg;
            this->crc = crc;
            new_cmd = true;
            return true;
        }

        new_cmd = false;
        return false;
    }

    const char id = 0x55;
    char cmd;
    char arg1;
    char crc;
    bool new_cmd;
};

AccelStepper stepper1(AccelStepper::DRIVER, 10, 8);
AccelStepper stepper2(AccelStepper::DRIVER, 10, 8);
AccelStepper stepper3(AccelStepper::DRIVER, 10, 8);

bool move_in_progress = false;
const int LedPin = 13;

// Commands
Command cmd;
const int cmd_led_on = 0;
const int cmd_led_off = 1;
const int cmd_fix_gripper_forward = 2;
const int cmd_fix_gripper_backward = 3;
const int cmd_gripper_forward = 4;
const int cmd_gripper_backward = 5;
const int cmd_plate_up = 6;
const int cmd_plate_down = 7;
const int cmd_fix_gripper_close = 8;
const int cmd_fix_gripper_open = 9;
const int cmd_gripper_close = 10;
const int cmd_gripper_open = 11;
const int cmd_rotate_gripper = 12;
const int cmd_rotate_plate = 13;

const int cmd_get_status = 100;

void setup()
{
    stepper1.setMaxSpeed(10000.0);
    stepper1.setAcceleration(1000.0);

    pinMode(LedPin, OUTPUT);
    Serial.begin(115200);

    pinMode(13, OUTPUT); // led
    pinMode(6, OUTPUT);  // enable

    cmd.new_cmd = false;
}

int state = 0;
long int cpt = 0;



// Fonctions
bool is_move_in_progress()
{
    //return stepper1.isRunning() || stepper2.isRunning() || stepper3.isRunning();
    return move_in_progress;
}

void move_fix_gripper_forward()
{ 
}

void move_fix_gripper_backward()
{ 
}

void move_gripper_forward()
{ 
}

void move_gripper_backward()
{ 
}

void move_plate_up()
{
    //stepper1.moveTo(1000);
    move_in_progress = true;
}

void move_plate_down()
{
    //stepper1.moveTo(-1000);
    move_in_progress = true;
}

void fix_gripper_close()
{ 
}

void fix_gripper_open()
{ 
}

void gripper_close()
{ 
}

void gripper_open()
{ 
}

void rotate_gripper(int rotation)
{ 
    if(rotation == rotation_90){
        //stepper1.moveTo(1000);
        move_in_progress = true;
    }
    else if(rotation == rotation_180){
        //stepper1.moveTo(1000);
        move_in_progress = true;
    }
    else if(rotation == rotation_270){
        //stepper1.moveTo(-1000);
        move_in_progress = true;
    }
}


void loop()
{

    while (Serial.available() >= 4)
    {
        char val = Serial.read(); // readString().toInt();
        char num = Serial.read();
        char arg1 = Serial.read();
        char crc = Serial.read();

        if (!cmd.set_command(val, num, arg1, crc))
        {
            Serial.write("0\n");
        }

        Serial.flush();
    }

    // Traitement des commandes
    if (cmd.new_cmd)
    {
        switch (cmd.cmd)
        {
        case cmd_led_on:
            digitalWrite(LedPin, HIGH);
            Serial.write("1\n"); // command ok
            break;

        case cmd_led_off:
            digitalWrite(LedPin, LOW);
            Serial.write("1\n"); // command ok
            break;

        case cmd_plate_down:
            move_plate_down();
            Serial.write("1\n"); // command ok
            break;

        case cmd_plate_up:
            move_plate_up();
            Serial.write("1\n"); // command ok
            break;

        case cmd_get_status:
          if(is_move_in_progress())
            Serial.write("2\n"); // move in progress
          else
            Serial.write("1\n"); // move finish
        }
    }
    cmd.new_cmd = false;

    // stepper1.move(move);

    stepper1.run();

    if(move_in_progress){
        cpt++;
        if(cpt > 65000){
            move_in_progress = false;
        }
    }
}
