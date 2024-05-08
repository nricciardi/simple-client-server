import socket


class Client:

    def __init__(self, family: socket.AddressFamily = socket.AF_INET, type: socket.SocketKind = socket.SOCK_STREAM, **other_socket_options):
        self.socket = socket.socket(family, type, **other_socket_options)

        print("client socket created")

    def connect(self, server_address: str, server_port: int):
        """
        connect client to server

        :param server_address:
        :param server_port:
        :return:
        """

        self.socket.connect((server_address, int(server_port)))

    def close(self):
        self.socket.close()
