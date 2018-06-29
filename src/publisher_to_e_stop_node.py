#!/usr/bin/env python

import rospy
from std_msgs.msg import Int8
#from gripper_control_youbot import GripperControl

rospy.init_node('e_stop_topic_publisher')

estop = rospy.Publisher('/command_e_stop', Int8, queue_size=10)
data_obj = Int8()

rate = rospy.Rate(10)
is_exit = True

count = 0
while (not rospy.is_shutdown()) and is_exit:

    input_value = raw_input('E-Stop: Press, Unpress, Status, Disco, '
                            'Warning, Warning off, Autonomous, Autonomous off, '
                            'LED off, Idle, Error, Exit: ')
    print input_value

    if input_value == 'Press' or input_value == 'p' or input_value == 'P':
        data_obj.data = 0
        estop.publish(data_obj)
        print data_obj.data

    elif input_value == 'Unpress' or input_value == 'u' or input_value == 'U':
        data_obj.data = 1
        estop.publish(data_obj)
        print data_obj.data

    elif input_value == 'Status'or input_value == 's' or input_value == 'S':
        data_obj.data = 2
        estop.publish(data_obj)
        print data_obj.data

    elif input_value == 'Exit' or input_value == 'e' or input_value == 'E':
        is_exit = False

    elif input_value == 'Disco'or input_value == 'd' or input_value == 'D':
        data_obj.data = 3
        estop.publish(data_obj)
        print data_obj.data

    elif input_value == 'Warning'or input_value == 'w' or input_value == 'W':
        data_obj.data = 4
        estop.publish(data_obj)
        print data_obj.data

    elif input_value == 'Idle'or input_value == 'i' or input_value == 'I':
        data_obj.data = 5
        estop.publish(data_obj)
        print data_obj.data

    elif input_value == 'Warning off'or input_value == 'wo' or input_value == 'WO':
        data_obj.data = 6
        estop.publish(data_obj)
        print data_obj.data

    elif input_value == 'Autonomous'or input_value == 'a' or input_value == 'A':
        data_obj.data = 7
        estop.publish(data_obj)
        print data_obj.data

    elif input_value == 'LED off'or input_value == 'o' or input_value == 'O':
        data_obj.data = 8
        estop.publish(data_obj)
        print data_obj.data

    elif input_value == 'Autonomous off'or input_value == 'ao' or input_value == 'AO':
        data_obj.data = 9
        estop.publish(data_obj)
        print data_obj.data

    elif input_value == 'Error'or input_value == 'r' or input_value == 'R':
        data_obj.data = 10
        estop.publish(data_obj)
        print data_obj.data

    else:
        print 'Not possible'
    #pubLED.publish(valueLED)
    rate.sleep()
