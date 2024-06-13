import socket


from server.singlethread_server import SingleThreadServer
from server.multithread_server import MultithreadServer
from server.multiprocess_fork_server import MultiprocessForkServer
from server.multiprocess_server import MultiprocessServer
from common.socket_utils import read_until_terminator_found, send_bytes
from common.cli import parse_args
import re


def server_handler(communication_socket: socket.socket, address: str):

    request: bytearray = read_until_terminator_found(communication_socket, b"\n---\n")

    request: str = request.decode("ascii")

    matches = re.findall(r"(\d{1,3}).(\d{1,3}).(\d{1,3}).(\d{1,3})", request)

    if len(matches) != 1:
        print("ERROR: bad request")
        send_bytes(communication_socket, b"ERROR\n---\n")
        return

    ip: (str, str, str, str) = matches[0]
    ip: (int, int, int, int) = (int(ip[0]), int(ip[1]), int(ip[2]), int(ip[3]))

    for i in ip:
        if i < 0 or i > 255:
            print("ERROR: bad request")
            send_bytes(communication_socket, b"ERROR\n---\n")
            return

    print(f"INFO: ip -> {ip}")

    # 0xxxx -> A
    # 10xxx -> B
    # 110xx -> C
    # 1110x -> D
    # 1111x -> E

    bit1 = ip[0] & 128 == 128
    bit2 = ip[0] & 64 == 64
    bit3 = ip[0] & 32 == 32
    bit4 = ip[0] & 16 == 16

    print(bit1, bit2, bit3, bit4)

    class_letter: str = None
    netaddr: str = None
    brdaddr: str = None

    if bit1 is False:       # A class
        class_letter = "A"
        netaddr = f"{ip[0]}.0.0.0"
        brdaddr = f"{ip[0]}.255.255.255"
    else:
        if bit2 is False:   # B class
            class_letter = "B"
            netaddr = f"{ip[0]}.{ip[1]}.0.0"
            brdaddr = f"{ip[0]}.{ip[1]}.255.255"
        else:
            if bit3 is False:   # C class
                class_letter = "C"
                netaddr = f"{ip[0]}.{ip[1]}.{ip[2]}.0"
                brdaddr = f"{ip[0]}.{ip[1]}.{ip[2]}.255"
            else:
                if bit4 is False:      # D class
                    class_letter = "D"
                else:                   # E class
                    class_letter = "E"


    if netaddr is None or brdaddr is None:
        response: str = f"{class_letter}"
    else:
        response: str = f"{class_letter} {netaddr} {brdaddr}\n---\n"

    send_bytes(communication_socket, response.encode("ascii"))

    print(f"INFO: sent response '{response}'")


if __name__ == '__main__':

    port = 1025

    sts = MultiprocessServer('127.0.0.1', port, handler=server_handler)

    sts.start()
