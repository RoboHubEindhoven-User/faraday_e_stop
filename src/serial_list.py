import glob
import serial


def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """

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

if __name__ == '__main__':
    serial_ports()

