import socket


from server.singlethread_server import SingleThreadServer
from server.multithread_server import MultithreadServer
from server.multiprocess_fork_server import MultiprocessForkServer
from server.multiprocess_server import MultiprocessServer
from common.socket_utils import read_until_terminator_found, send_bytes, send_file, receive_file
from common.cli import parse_args
import re


def server_handler(communication_socket: socket.socket, address: str):

    header: bytearray = read_until_terminator_found(communication_socket, b'\n---\n')

    header: str = header.decode("ascii")

    matches = re.findall(r"File:(.*)", header)

    if len(matches) != 1:
        print("ERROR: bad request")
        return

    filepath: str = matches[0]

    print(f"INFO: receive file: {filepath}")

    filepath: str = f"{matches[0]}.bkp"

    receive_file(communication_socket, filepath)

    print(f"INFO: file received")


if __name__ == '__main__':

    args = parse_args(1)

    port = args[1]

    sts = MultiprocessForkServer('127.0.0.1', port, handler=server_handler)

    sts.start()
