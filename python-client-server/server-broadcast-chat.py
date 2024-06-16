import socket
from typing import Tuple
from server.singlethread_server import SingleThreadServer
from server.multithread_server import MultithreadServer
from server.multiprocess_fork_server import MultiprocessForkServer
from server.multiprocess_server import MultiprocessServer
from common.socket_utils import read_until_terminator_found, send_bytes
from common.cli import parse_args
import re


ONLINE_CLIENT = {}


def send_all(from_client: str, message: bytes):
    for cid, csocket in ONLINE_CLIENT.items():

        if cid == from_client:
            continue

        print(f"INFO: send to {cid}")

        send_bytes(csocket, message)

def server_handler(communication_socket: socket.socket, address: Tuple[str, int]):

    print(f"INFO: client {address} connected")

    client_id: str = f"{address[0]}:{address[1]}"

    ONLINE_CLIENT[client_id] = communication_socket

    send_bytes(communication_socket, f"Welcome {client_id}\n".encode("ascii"))
    print("INFO: welcome sent")

    print("INFO: sending to other clients 'join'...")
    send_all(client_id, f"{client_id} joined...\n".encode("ascii"))

    while True:
        request: bytearray = read_until_terminator_found(communication_socket, b'\n', include_terminator=True, raise_on_disconnect=False)

        if request is None:
            print(f"INFO: client {client_id} left")
            ONLINE_CLIENT.pop(client_id)
            send_bytes(communication_socket, f"Bye {client_id}\n".encode("ascii"))

            print("INFO: sending to other clients 'left'...")
            send_all(client_id, f"{client_id} left...\n".encode("ascii"))

            break

        else:

            print("INFO: sending to other clients...")

            message: str = f"[{client_id}]: {request.decode('ascii')}"

            send_all(client_id, message.encode("ascii"))


if __name__ == '__main__':

    # args = parse_args(1)

    port = 12345 # args[1]

    sts = MultithreadServer('127.0.0.1', port, handler=server_handler)

    sts.start()
