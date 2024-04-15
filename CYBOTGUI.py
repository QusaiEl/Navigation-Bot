# PRoblems:plotting fixed but is inaccurate: maybe use ping && change units on graph, 
# need to implement bump/ cliff returns, infinite loop on move commands
# Date: 10/30/2023
# Description: Client starter code that combines: 1) Simple GUI, 2) creation of a thread for
#              running the Client socket in parallel with the GUI, and 3) Simple recieven of mock sensor 
#              data for a server/cybot.for collecting data from the cybot.

# General Python tutorials (W3schools):  https://www.w3schools.com/python/

# Serial library:  https://pyserial.readthedocs.io/en/latest/shortintro.html 
import serial
import time # Time library   
# Socket library:  https://realpython.com/python-sockets/  
# See: Background, Socket API Overview, and TCP Sockets  
import socket
import tkinter as tk # Tkinter GUI library
# Thread library: https://www.geeksforgeeks.org/how-to-use-thread-in-tkinter-python/
import threading
import os  # import function for finding absolute path to this python script
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np
import matplotlib.pyplot as plt

##### START Define Functions  #########
# Main: Mostly used for setting up, and starting the GUI
def main():

        global window  # Made global so quit function (send_quit) can access
        window = tk.Tk() # Create a Tk GUI Window
        window.title("Store Bot")
        window.geometry('700x700')

        # Last command label  
        global Last_rx_Label  # Made global so that Client function (socket_thread) can modify
        Last_rx_Label = tk.Label(text="rx: ") # Creat a Label
        Last_rx_Label.pack() # Pack the label into the window for display

        # Quit command Button
        quit_command_Button = tk.Button(fg='white',bg='red',text ="Quit", command = send_quit)
        quit_command_Button.pack(side=tk.BOTTOM)  # Pack the button into the window for display

        # Cybot Scan command Button
        scan_command_Button = tk.Button(text ="Press to Scan", command = send_scan)
        scan_command_Button.place(x=310, y=100) # Pack the button into the window for display

        move_command_Button_5 = tk.Button(text = "-5", command = send_move_m5)
        move_command_Button_5.place(x=550, y=40)

          # Send move Button
        move_command_Button_5 = tk.Button(text = "+5", command = send_move_5)
        move_command_Button_5.place(x=550, y=70)

        # Send move Button
        move_command_Button_10 = tk.Button(text = "+10", command = send_move_10)
        move_command_Button_10.place(x=550, y=100)

        # Send turn Button
        turn_command_Button = tk.Button(text = "left 10", command = send_turn_r10)
        turn_command_Button.place(x=50, y=100)

        turn_command_Button = tk.Button(text = "left 45", command = send_turn_r45)
        turn_command_Button.place(x=50, y=70)

        turn_command_Button = tk.Button(text = "left 90", command = send_turn_r90)
        turn_command_Button.place(x=50, y=40)

        turn_command_Button = tk.Button(text = "right 10", command = send_turn_l10)
        turn_command_Button.place(x=110, y=100)

        turn_command_Button = tk.Button(text = "right 45", command = send_turn_l45)
        turn_command_Button.place(x=110, y=70)

        turn_command_Button = tk.Button(text = "right 90", command = send_turn_l90)
        turn_command_Button.place(x=110, y=40)

        music_button = tk.Button(text="play music", command=play_music)
        music_button.pack(side=tk.TOP)

        #plot output
        angle_degrees = [] # Initially empty

        angle_radians = [] # Initially empty

        distance = []      # Initially empty

        distanceir = [] 
        
        global fig, ax
        global canvas
        fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}) # One subplot of type polar
        canvas = FigureCanvasTkAgg(fig, master = window)
        ax.plot(angle_radians, distance, color='r', linewidth=4.0)  # Plot distance verse angle (in radians), using red, line width 4.0
        ax.set_xlabel('Distance (m)', fontsize = 14.0)  # Label x axis
        ax.set_ylabel('Angle (degrees)', fontsize = 14.0) # Label y axis
        ax.xaxis.set_label_coords(0.5, 0.15) # Modify location of x axis label (Typically do not need or want this)
        ax.tick_params(axis='both', which='major', labelsize=9) # set font size of tick labels
        ax.set_rmax(2.0)                    # Saturate distance at 2.5 meters
        ax.set_rticks([.3, .6, .9, 1.2, 1.5])     # Set plot "distance" tick marks at .5, 1, 1.5, 2, and 2.5 meters
        ax.set_rlabel_position(-22.5)     # Adjust location of the radial labels
        ax.set_thetamax(180)              # Saturate angle to 180 degrees
        ax.set_xticks(np.arange(0,np.pi+.1,np.pi/4)) # Set plot "angle" tick marks to pi/4 radians (i.e., displayed at 45 degree) increments
                                             # Note: added .1 to pi to go just beyond pi (i.e., 180 degrees) so that 180 degrees is displayed
        ax.grid(True)                     # Show grid lines
        canvas.get_tk_widget().pack(side=tk.BOTTOM)
       

        

        # Create a Thread that will run a fuction assocated with a user defined "target" function.
        # In this case, the target function is the Client socket code
        my_thread = threading.Thread(target=socket_thread) # Creat the thread
        my_thread.start() # Start the thread

        # Start event loop so the GUI can detect events such as button clicks, key presses, etc.
        window.mainloop()


# Quit Button action.  Tells the client to send a Quit request to the Cybot, and exit the GUI
def send_quit():
        global gui_send_message
        global window  # Main GUI window

        gui_send_message = "quit\n"   # Update the message for the Client to send
        time.sleep(1)
        window.destroy() # Exit GUI


# Scan Button action.  Tells the client to send a scan request to the Cybot
def send_scan():
        global gui_send_message # Command that the GUI has requested sent to the Cybot
        gui_send_message = "m\n"   # Update the message for the Client to send

# move button takes input from text box to move the cybot
def send_move_10():
        global gui_send_message
        gui_send_message = "w\n"

def send_move_5():
        global gui_send_message
        gui_send_message = "e\n"

def send_move_m5():
        global gui_send_message
        gui_send_message = "r\n"

# turn button takes input from text box to move the cybot
def send_turn_r10():
        global gui_send_message
        gui_send_message = "p\n"

def send_turn_r45():
        global gui_send_message
        gui_send_message = "i\n"

def send_turn_r90():
        global gui_send_message
        gui_send_message = "y\n"

def send_turn_l10():
        global gui_send_message
        gui_send_message = "o\n"

def send_turn_l45():
        global gui_send_message
        gui_send_message = "u\n"

def send_turn_l90():
        global gui_send_message 
        gui_send_message = "t\n"

def play_music():
        global gui_send_message
        gui_send_message = "x\n"

def plot():
        absolute_path = os.path.dirname(__file__) # Absoult path to this python script
        relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script
        full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file
        filename = 'sensor-scan.txt' # Name of sensor data file

        angle_degrees = [] # Initially empty

        angle_radians = [] # Initially empty

        distance = []      # Initially empty

        distanceping = []

# Open file containing CyBot sensor scan from 0 - 180 degrees
        file_object = open(full_path + filename,'r') # Open the file: file_object is just a variable for the file "handler" returned by open()
        file_header = file_object.readline() # Read and store the header row (i.e., 1st row) of the file into file_header
        file_data = file_object.readlines()  # Read the rest of the lines of the file into file_data
        file_object.close() # Important to close file one you are done with it!!

# For each line of the file split into columns, and assign each column to a variable
        for line in file_data: 
                if(line == "END\n"):
                        break
                data = line.split()    # Split line into columns (by default delineates columns by whitespace)
                angle_degrees.append(float(data[0]))  # Column 0 holds the angle at which distance was measured
                distance.append(float(data[1]) / 100)       # Column 1 holds the distance that was measured at a given angle 
                if(float(data[2]) < 50):
                        distanceping.append(float(data[2]) / 100)
                else:       
                        distanceping.append(99999)      


# Convert python sequence (list of strings) into a numpy array
        angle_degrees = np.array(angle_degrees) # Avoid "TypeError: can't multiply sequence by non-int of type float"
                                        # Link for more info: https://www.freecodecamp.org/news/typeerror-cant-multiply-sequence-by-non-int-of-type-float-solved-python-error/
                                        
        angle_radians = (np.pi/180) * angle_degrees # Convert degrees into radians

# Create a polar plot
        ax.clear()
        ax.plot(angle_radians, distance, color='r', linewidth=4.0)  # Plot distance verse angle (in radians), using red, line width 4.0
        # ax.plot(angle_radians, distanceping, color = 'b', linewidth=4.0)
        ax.set_xlabel('Distance (m)', fontsize = 14.0)  # Label x axis
        ax.set_ylabel('Angle (degrees)', fontsize = 14.0) # Label y axis
        ax.xaxis.set_label_coords(0.5, 0.15) # Modify location of x axis label (Typically do not need or want this)
        ax.tick_params(axis='both', which='major', labelsize=9) # set font size of tick labels
        ax.set_rmax(2.0)                    # Saturate distance at 2.5 meters
        ax.set_rticks([0.3, .6, .9, 1.2, 1.5])   # Set plot "distance" tick marks at .5, 1, 1.5, 2, and 2.5 meters
        ax.set_rlabel_position(-22.5)     # Adjust location of the radial labels
        ax.set_thetamax(180)              # Saturate angle to 180 degrees
        ax.set_xticks(np.arange(0,np.pi+.1,np.pi/4)) # Set plot "angle" tick marks to pi/4 radians (i.e., displayed at 45 degree) increments
                                             # Note: added .1 to pi to go just beyond pi (i.e., 180 degrees) so that 180 degrees is displayed
        ax.grid(True)                     # Show grid lines

# Create title for plot (font size = 14pt, y & pad controls title vertical location)
        ax.set_title("Current Scan", size=14, y=1.0, pad=-24) 
        canvas.draw()  # Display plot


# Client socket code (Run by a thread created in main)
def socket_thread():
        # Define Globals
        global Last_rx_Label # GUI label for displaying the last command sent to the Cybot
        global gui_send_message   # Command that the GUI has requested be sent to the Cybot

        # A little python magic to make it more convient for you to adjust where you want the data file to live
        # Link for more info: https://towardsthecloud.com/get-relative-path-python 
        absolute_path = os.path.dirname(__file__) # Absoult path to this python script
        relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script)
        full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file
        filename = 'sensor-scan.txt' # Name of file you want to store sensor data from your sensor scan command

        # Choose to create either a UART or TCP port socket to communicate with Cybot (Not both!!)
        # UART BEGIN
        #cybot = serial.Serial('COM100', 115200)  # UART (Make sure you are using the correct COM port and Baud rate!!) ?
        # UART END

        # TCP Socket BEGIN (See Echo Client example): https://realpython.com/python-sockets/#echo-client-and-server
        HOST = "192.168.1.1"  # The server's hostname or IP address
        PORT = 288        # The port used by the server
        cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
        cybot_socket.connect((HOST, PORT))   # Connect to the socket  (Note: Server must first be running)
                      
        cybot = cybot_socket.makefile("rbw", buffering=0)  # makefile creates a file object out of a socket:  https://pythontic.com/modules/socket/makefile
        # TCP Socket END

        command_display = ""
        send_message = ''   # 2) Have user enter text
        gui_send_message = "wait\n"  # Initialize GUI command message to wait                                
        rx_message = ""
        # cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server

        # print("Sent to server: " + send_message) 

        # Send messges to server until user sends "quit"
        while send_message != 'q\n':
                command_display = ""
                send_message = '' 
                rx_message = ""
                Last_rx_Label.config(text = command_display) 
                # Update the GUI to display command being sent to the CyBot
                
                 
        
                # Check if a sensor scan command has been sent
                if (gui_send_message == "m\n") or (gui_send_message == "M\n"):
                        print("Requested Sensor scan from Cybot:\n")
                        rx_message = bytearray(1) # Initialize a byte array

                        # Create or overwrite existing sensor scan data file
                        file_object = open(full_path + filename,'w') # Open the file: file_object is just a variable for the file "handler" returned by open()

                        while (rx_message.decode() != "END\n"): # Collect sensor data until "END" recieved
                                rx_message = cybot.readline()   # Wait for sensor response, readline expects message to end with "\n"
                                file_object.write(rx_message.decode())  # Write a line of sensor data to the file
                                print(rx_message.decode()) # Convert message from bytes to String (i.e., decode), then print



                        file_object.close() # Important to close file once you are done with it!!                
                        plot()
                        gui_send_message = "wait\n"
                elif (gui_send_message == "w\n") or (gui_send_message == "e\n"):
                        rx_message = cybot.readline()
                        command_display = "rx:" + rx_message.decode()
                        Last_rx_Label.config(text = command_display) 
                        gui_send_message = "wait\n"
                else:     
                        gui_send_message = "wait\n"
                        print("")
                        # rx_message = cybot.readline()      # Wait for a message, readline expects message to end with "\n"
                        # print("Got a message from server: " + rx_message.decode() + "\n") # Convert message from bytes to String (i.e., decode)


                # Choose either: 1) Idle wait, or 2) Request a periodic status update from the Cybot
                # 1) Idle wait: for gui_send_message to be updated by the GUI
                while gui_send_message == "wait\n": 
                        time.sleep(.1)  # Sleep for .1 seconds
                send_message = gui_send_message
        
                


                # 2) Request a periodic Status update from the Cybot:
                # every .1 seconds if GUI has not requested to send a new command
                #time.sleep(.1)
                #if(gui_send_message == "wait\n"):   # GUI has not requested a new command
                #        send_message = "status\n"   # Request a status update from the Cybot
                #else:
                #        send_message = gui_send_message  # GUI has requested a new command
        
                cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server need
                
        print("Client exiting, and closing file descriptor, and/or network socket\n")
        time.sleep(2) # Sleep for 2 seconds
        cybot.close() # Close file object associated with the socket or UART
        cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)

##### END Define Functions  #########


### Run main ###
main()
