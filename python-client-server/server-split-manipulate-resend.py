import socket


from server.singlethread_server import SingleThreadServer
from server.multithread_server import MultithreadServer
from server.multiproces_server import MultiprocessServer
from common.socket_utils import read_until_terminator_found, send_bytes
from common.cli import parse_args
import re


def server_handler(communication_socket: socket.socket, address: str):

    result: bytearray = read_until_terminator_found(communication_socket, b'\n')

    result: str = result.decode("ascii")

    matches = re.findall(r"Value:(.*)", result)

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
