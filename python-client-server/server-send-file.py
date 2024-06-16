import socket
import sys
from typing import Tuple
from server.singlethread_server import SingleThreadServer
from server.multithread_server import MultithreadServer
from server.multiprocess_fork_server import MultiprocessForkServer
from server.multiprocess_server import MultiprocessServer
from common.socket_utils import read_until_terminator_found, send_bytes, send_file
from common.cli import parse_args
import re


def server_handler(communication_socket: socket.socket, address: Tuple[str, int]):

    ### TO OBTAIN FILEPATH FROM CLIENT

    # result: bytearray = read_until_terminator_found(communication_socket, b'\n')
    #
    # result: str = result.decode("ascii")
    #
    # matches = re.findall(r"File:(.*)", result)
    #
    # if len(matches) != 1:
    #     print("ERROR: bad request")
    #     return
    #
    # filepath: str = matches[0]

    filepath: str = "test.txt"

    print(f"INFO: request file: {filepath}")

    send_bytes(communication_socket, f"File:{filepath}\n---\n".encode("ascii"))
    send_file(communication_socket, filepath)

    print(f"INFO: sent response file")

    sys.exit(0)


if __name__ == '__main__':

    args = parse_args(1)

    port = args[1]

    sts = MultiprocessForkServer('127.0.0.1', port, handler=server_handler)

    sts.start()
