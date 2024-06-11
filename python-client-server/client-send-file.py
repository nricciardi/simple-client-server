import socket


from client.client import Client
from common.socket_utils import read_until_terminator_found, send_bytes, send_file
from common.cli import parse_args
import re


def client_elaboration(c: Client) -> None:

    filepath: str = "test.txt"

    request: str = f"File:{filepath}\n---\n"

    send_bytes(c.socket, request.encode("ascii"))
    send_file(c.socket, filepath)


def main() -> None:

    args = parse_args(1)

    port = args[1]

    c = Client()

    c.connect('127.0.0.1', port)

    client_elaboration(c)


if __name__ == '__main__':
    main()
