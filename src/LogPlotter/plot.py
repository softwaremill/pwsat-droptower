import matplotlib.pyplot as plt
import argparse
import numpy

parser = argparse.ArgumentParser(description='Parse log file.')
parser.add_argument('log', metavar='N', type=str, nargs='+',
                    help='File to parse.')

args = parser.parse_args()
file_name = str(args.log).replace("[\'","").replace("\']","")

file = open(file_name,'r')
file_to_parse = file.read()
file.close()

file_lines = file_to_parse.split("\n")

records_time = []
records_accel_x = []
records_accel_y = []
records_accel_z = []
records_ffall_flag = []
records_switch_flag = []
records_state_flag = []

for i in range(0, len(file_lines)):
    if len(file_lines[i].split(",")) == 7:
        try:
            int(file_lines[i][0])
        except:
            pass
        else:
            records_time.append(file_lines[i].split(",")[0])
            records_accel_x.append(file_lines[i].split(",")[1])
            records_accel_y.append(file_lines[i].split(",")[2])
            records_accel_z.append(file_lines[i].split(",")[3])
            records_ffall_flag.append(file_lines[i].split(",")[4])
            records_switch_flag.append(file_lines[i].split(",")[5])
            records_state_flag.append(file_lines[i].split(",")[6])

fig = plt.figure(1) 
fig.canvas.set_window_title('PW-Sat2 DropTower log file:' + file_name)
plt.subplot(411)
plt.plot(records_time, records_accel_x, 'r', label="Acceleration X")
plt.ylabel('Accel')
plt.hold(True)
plt.plot(records_time, records_accel_y, 'g', label="Acceleration Y")
plt.plot(records_time, records_accel_z, 'b', label="Acceleration Z")
plt.legend(loc=1, framealpha=0.7, fontsize=8)
plt.subplot(412)
plt.plot(records_time, records_ffall_flag, 'm', label="Free Fall Flag")
plt.ylabel('Free Fall')
plt.legend(loc=1, framealpha=0.7, fontsize=8)
plt.subplot(413)
plt.plot(records_time, records_switch_flag, 'y', label="Switch Flag")
plt.ylabel('Switch')
plt.legend(loc=1, framealpha=0.7, fontsize=8)
plt.subplot(414)
plt.plot(records_time, records_state_flag, 'c', label=" State Flag")
plt.ylabel('State')
plt.legend(loc=1, framealpha=0.7, fontsize=8)
plt.xlabel('Time [ms]')

fig = plt.figure(2) 
plt.hold(False)
plt.plot(records_time, 'r', label="Time [ms]")

plt.show()