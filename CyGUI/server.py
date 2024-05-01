import asyncio
import socket
import time
import websockets

HOST = "192.168.1.1"  # The server's hostname or IP address
PORT = 288  # The port used by the server
cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
cybot_socket.connect((HOST, PORT))  # Connect to the socket

print("Connected to cyBOT, opening WebSocket for GUI now")

async def send_with_echo(cybot_socket, char):
    print("Sending ", char)
    try:
        cybot_socket.sendall(char.encode())  # Send the character
        start_time = time.time()  # Record the start time
        received_successfully = False  # Flag to track whether the character was received successfully
        while True:
            rx_message = cybot_socket.recv(1024).decode().strip()  # Read the received message
            print("Received:", rx_message)  # Debug: Print received message
            if rx_message and rx_message[0] == char and len(rx_message) > 1:  # If the received message matches the sent character
                print("Received confirmation, sending next character")
                received_successfully = True
                break  # Exit the loop
            # if time.time() - start_time > 1:  # If one second has passed without receiving the echo
            print("Resending the character")
            cybot_socket.sendall(char.encode())  # Resend the character
            start_time = time.time()  # Reset the start time for the next one-second interval
            await asyncio.sleep(0.1)  # Wait for a short time before checking again

        # If the character was received successfully, send it back to the WebSocket client
        if received_successfully:
            return char
        else:
            return None
    except Exception as e:
        print("Error:", e)
        return None


async def server(websocket, path):
    print("WebSocket connected")
    await websocket.send("Client connected to server")
    async for message in websocket:
        print("Received message:", message)
        print("Sending to cyBOT...")
        for char in message:
            await send_with_echo(cybot_socket, char)  # Send each character and wait for its echo
            await asyncio.sleep(0.25)
        print()
        print("Sent to cyBOT")
        print("Awaiting Echo")
        if message[0] != 'j':
            cybot_socket.settimeout(15.0)
            try:
                rx_message = cybot_socket.recv(1024).decode().strip()  # Read the received message
            except UnicodeDecodeError:
                print("Error decoding part of the message. Ignoring...")
                continue  # Ignore the paßt that cannot be decoded and continue listening
            await websocket.send(rx_message)
            print("From cyBOT:", rx_message)
        else:
            while True:
                try:
                    cybot_socket.settimeout(5.0)
                    rx_message = cybot_socket.recv(1024).decode().strip()  # Read the received message
                    print("Received:", rx_message)  # Debug: Print received message
                    await websocket.send(rx_message)
                    print("From cyBOT:", rx_message)
                except UnicodeDecodeError:
                    print("Error decoding part of the message. Ignoring...")
                    continue  # Ignore the part that cannot be decoded and continue listening
                except socket.timeout:
                    print("Timeout: No message received for 5 seconds.")
                    break  # Exit the loop if no message is received for 1 second



start_server = websockets.serve(server, "0.0.0.0", 8080)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
