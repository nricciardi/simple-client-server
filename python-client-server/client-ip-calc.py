import client.client
from client.client import Client
from common.socket_utils import read_until_terminator_found, send_bytes
from common.cli import parse_args
import re


def client_elaboration(c: Client, ip: str) -> None:

    send_bytes(c.socket, f"{ip}\n---\n".encode("ascii"))

    response: bytearray = read_until_terminator_found(c.socket, b'\n---\n')

    print(f"INFO: response: {response.decode('ascii')}")


def main() -> None:

    args = parse_args(2)

    server_addr = args[1]
    ip = args[2]
    port = 1025

    c = Client()

    c.connect(str(server_addr), port)

    client_elaboration(c, str(ip))


if __name__ == '__main__':
    main()
