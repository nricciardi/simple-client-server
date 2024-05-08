import socket
from abc import ABC
from typing import Callable

DEFAULT_CLIENT_QUEUE_SIZE = 1



class Server(ABC):

    def __init__(self, address: str, port: int, family: socket.AddressFamily = socket.AF_INET, type: socket.SocketKind = socket.SOCK_STREAM, **other_socket_options):
        self.address = address
        self.port = port

        self.socket = socket.socket(family, type, **other_socket_options)
        self.running = False

        print(f"server socket created")

    def start(self, client_queue_size: int = DEFAULT_CLIENT_QUEUE_SIZE):
        self.socket.bind((self.address, int(self.port)))

        self.socket.listen(client_queue_size)

        print(f"server socket bound and listening on {self.address}:{self.port}")

        self.running = True
        print("server start")

        self.on_start()

    def on_start(self):
        pass

    def stop(self):
        self.running = False
        print("server stop")

        self.on_stop()

    def on_stop(self):
        pass

    def close(self):
        self.socket.close()
