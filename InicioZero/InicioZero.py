from pydynamixel import dynamixel

# You'll need to change this to the serial port of your USB2Dynamixel
serial_port = '/dev/ttyUSB0'

# You'll need to modify these for your setup
target_position = 512 # (range: 0 to 1023)

# If this is the first time the robot was powered on,
# you'll need to read and set the current position.
# (See the documentation above.)
first_move = True

ser = dynamixel.get_serial_for_url(serial_port, baudrate=1000000)

for i in range(1,19):
  dynamixel.init(ser, i)

for i in range(5,6):
  try:
    p = dynamixel.get_position(ser, i, num_error_attempts=10)
    print "motor", i, "pos" ,p
    dynamixel.set_position(ser, i, 512)
    dynamixel.send_action_packet(ser)
    #print('Success!')

  except Exception as e:
     #print('Unable to move to desired position.')
     #print(e)
     pass

