import socket
from concurrent.futures import ThreadPoolExecutor
from typing import Callable

from .server import Server


def default_handler(communication_socket, address):
    print(f"default handler function: new connection from {address}: {communication_socket}")


class MultithreadServer(Server):

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

        with ThreadPoolExecutor() as tpe:
            while self.running:
                communication_socket, address = self.socket.accept()

                print(f"INFO: accepted new connection from {address}")

                result = tpe.submit(self.handler, communication_socket, address)

                result.result()
