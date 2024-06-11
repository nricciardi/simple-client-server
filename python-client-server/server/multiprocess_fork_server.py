import multiprocessing
import socket
import sys
import time
from typing import Callable
import os

from .server import Server


def default_handler(communication_socket, address):
    print(f"process: {os.getpid()}")
    print(f"default handler function: new connection from {address}: {communication_socket}")


class MultiprocessForkServer(Server):

    def __init__(self, address: str, port: int, handler: Callable[[socket.socket, str], None] = default_handler, **other_socket_options):
        """

        :param address: server address
        :param port: server port
        :param handler: function to handle client requests
        :param other_socket_options:
        """

        super().__init__(address, port, **other_socket_options)

        self.handler = handler

    def on_start(self):

        children = []

        while self.running:
            communication_socket, address = self.socket.accept()

            print(f"INFO: server (father {os.getpid()}) accepted new connection from {address}")

            pid = os.fork()

            if pid == 0:     # child

                self.handler(communication_socket, address)
                sys.exit(0)

            else:           # father

                print(f"INFO: request is handled in child process: {pid}")

                communication_socket.close()
                print(f"INFO: communication socket of child {pid} was closed")
