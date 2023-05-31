
import serial
import time
from enum import Enum

def send_command_move(cmd, data):
    rep = send_command(cmd, data)
    if '1' in rep :
        in_progress = True
        while in_progress :
            time.sleep(0.1)
            rep = get_status()
            if '1' in rep :
                in_progress = False

def send_command(cmd, data):
    id = 0x55
    checksum = id + cmd + data
    nbr = ser.write([id,cmd,data,checksum])
    print("Send command : {0} - {1} / crc : {2}".format(cmd, nbr, checksum))

    rep = str(ser.readline())

    print(rep)
    return rep

# Low level commands
def led_on():
    send_command(0, 0)

def led_off():
    send_command(1, 0)

def fix_grip_forward(number):
    send_command_move(2, number)

def fix_grip_backward():
    send_command_move(3, 0)

def grip_forward(number):
    send_command_move(4, number)

def grip_backward():
    send_command_move(5, 0)

def move_plate_up():
    send_command_move(6, 0)

def move_plate_down():
    send_command_move(7, 0)

def fix_grip_close():
    send_command_move(8, 0)

def fix_grip_open():
    send_command_move(9, 0)

def grip_close():
    send_command_move(10, 0)

def grip_open():
    send_command_move(11, 0)

def angle_to_rotation_cmd(angle):
    if angle == 90 :
        return 0
    elif angle == 180 :
        return 1
    elif angle == 270 :
        return 2

def rotate_gripper(rotation):
    tmp = angle_to_rotation_cmd(rotation)
    send_command_move(12, tmp)

def rotate_plate(rotation):
    tmp = angle_to_rotation_cmd(rotation)
    send_command_move(13, tmp)


def get_status():
    return send_command(100, 0)

# mid level commands
def move_grippers(left_number):
    grip_open()
    fix_grip_open()
    grip_backward()
    fix_grip_backward()
    grip_forward(left_number)
    grip_close()
    
    if left_number >= 1 and left_number <= 3 :
        fix_grip_forward(5 - left_number)
    else :
        raise Exception("Invalid number of rows")

    fix_grip_close()


def rotate_cube_vertical(rotation):
    move_plate_up()
    grip_open()
    fix_grip_open()
    grip_backward()
    fix_grip_backward()
    grip_forward(3)
    grip_close()
    rotate_plate(rotation)


def rotate_cube_horizontal(rotation):
    move_plate_up()
    grip_open()
    fix_grip_open()
    grip_backward()
    fix_grip_backward()
    rotate_plate(rotation)


def rotate_left(number, rotation):
    move_plate_up()
    move_grippers(number)
    move_plate_down()
    rotate_gripper(rotation)
    
# Top level commands


def rotate_couronne(number, rotation):
    if number > 0 and number <= 3 :
        rotate_left(number, rotation)
    else :
        rotate_cube_horizontal(180)
        rotate_left(6-number, 360 - rotation)
        rotate_cube_horizontal(180)


ser = serial.Serial('COM4', 115200, timeout=0.1)
print(ser.name)


print("wait reset")
time.sleep(1)


# led off
led_off()
time.sleep(3)

# led on 
led_on()
time.sleep(3)

print("Move plate up")


print("Move plate down")


ser.close()
