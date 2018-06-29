#!/usr/bin/env python

import rospy
import serial
import glob
from std_msgs.msg import Int8

def serial_ports():

    # this excludes your current terminal "/dev/tty"
    ports = glob.glob('/dev/ttyACM[0-9]*')

    result = []
    for port in ports:
        print port
        try:
            ser = serial.Serial(port)
            ser.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

ports = serial_ports()
ser = serial.Serial(ports[0], 115200)

# use "Serial.parseInt()" in Arduino to read the integers


def write2estop(msg):
    state_estop = msg.data
    ser.write(chr(state_estop- (256*int(state_estop/127))))


if __name__ == '__main__':

    rospy.init_node('estop_controller')
    print 'Ready to get a command!'
    rospy.Subscriber('/command_e_stop', Int8, write2estop)
    rospy.Subscriber('/value_e_stop', Int8)
    pub = rospy.Publisher('/value_e_stop', Int8, queue_size=10)
    pub_obj = Int8()

    while not rospy.is_shutdown():
        try:
            dat = ord(ser.read(1))
            if dat == 0:
                print "E-stop = Unpressed, Ready to go"
            if dat == 1:
                print "E-stop = Pressed, All secondary functions are down"
            pub_obj.data = dat
            pub.publish(pub_obj)

        except serial.SerialException, e:
            pass
