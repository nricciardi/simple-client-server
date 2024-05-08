import socket

from client.client import Client
from server.singlethread_server import SingleThreadServer
from server.multithread_server import MultithreadServer


def server_handler(communication_socket: socket.socket, address: str):

    data = communication_socket.recv(1024)

    print(f"{data}")

    communication_socket.send(data)


if __name__ == '__main__':
    sts = MultithreadServer('127.0.0.1', 12345, handler=server_handler)

    sts.start()
