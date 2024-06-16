import socket
from typing import Tuple
from server.singlethread_server import SingleThreadServer
from server.multithread_server import MultithreadServer
from server.multiprocess_fork_server import MultiprocessForkServer
from server.multiprocess_server import MultiprocessServer
from common.socket_utils import read_until_terminator_found, send_bytes
from common.cli import parse_args
import re


def server_handler(communication_socket: socket.socket, address: Tuple[str, int]):

    request: bytearray = read_until_terminator_found(communication_socket, b'\n')

    request: str = request.decode("ascii")

    matches = re.findall(r"Value:(.*)", request)

    if len(matches) != 1:
        print("ERROR: bad request")
        return

    body: str = matches[0]

    print(f"INFO: request body: {body}")

    response: str = f"Value:{body.upper()}\n"

    send_bytes(communication_socket, response.encode("ascii"))

    print(f"INFO: sent response '{response}'")


if __name__ == '__main__':

    args = parse_args(1)

    port = args[1]

    sts = MultiprocessServer('127.0.0.1', port, handler=server_handler)

    sts.start()
