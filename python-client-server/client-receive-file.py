import socket


from client.client import Client
from common.socket_utils import read_until_terminator_found, send_bytes, send_file, receive_file
from common.cli import parse_args
import re


def client_elaboration(c: Client) -> None:
    header: bytearray = read_until_terminator_found(c.socket, b'\n---\n')

    header: str = header.decode("ascii")

    matches = re.findall(r"File:(.*)", header)

    if len(matches) != 1:
        print("ERROR: bad request")
        return

    filepath: str = matches[0]

    print(f"INFO: receive file: {filepath}")

    filepath: str = f"{matches[0]}.bkp"

    receive_file(c.socket, filepath)

    print(f"INFO: file received")


def main() -> None:

    args = parse_args(1)

    port = args[1]

    c = Client()

    c.connect('127.0.0.1', port)

    client_elaboration(c)


if __name__ == '__main__':
    main()
