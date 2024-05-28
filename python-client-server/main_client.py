import client.client
from client.client import Client
from common.socket_utils import read_until_terminator_found, send_bytes
from common.cli import parse_args
import re


def client_elaboration(c: Client) -> None:

    send_bytes(c.socket, "Value:hello\n".encode("ascii"))

    response: bytearray = read_until_terminator_found(c.socket, b'\n')

    print(f"INFO: response: {response.decode('ascii')}")


def main() -> None:

    args = parse_args(1)

    port = args[1]

    c = Client()

    c.connect('127.0.0.1', port)

    client_elaboration(c)


if __name__ == '__main__':
    main()
