import socket
from typing import Tuple
from server.singlethread_server import SingleThreadServer
from server.multithread_server import MultithreadServer
from server.multiprocess_fork_server import MultiprocessForkServer
from server.multiprocess_server import MultiprocessServer
from common.socket_utils import read_until_terminator_found, send_bytes
from common.cli import parse_args
import re


def and_bit_bit(v1: str, v2: str) -> str:

    result = []

    for i in range(len(v1)):
        a = int(v1[i]) & int(v2[i])
        result.append(str(a))

    return "".join(result)

def or_bit_bit(v1: str, v2: str) -> str:

    result = []

    for i in range(len(v1)):
        a = int(v1[i]) | int(v2[i])
        result.append(str(a))

    return "".join(result)

def dec_to_bin(n: int) -> str:
    return str(bin(int(n))[2:]).zfill(8)


def bin_to_dec(b: str) -> int:
    return int(b, 2)


def from_ip_bin_to_ip_dec(ip: (str, str, str, str)) -> str:
    return f"{bin_to_dec(ip[0])}.{bin_to_dec(ip[1])}.{bin_to_dec(ip[2])}.{bin_to_dec(ip[3])}"


def build_netmask(m: int) -> (str, str, str, str):
    plain_mask: str = "".zfill(32)

    for i in range(m):
        plain_mask = plain_mask.replace("0", "1", 1)

    return plain_mask[0:8], plain_mask[8:16], plain_mask[16:24], plain_mask[24:32]


def build_wildmask(m: int) -> (str, str, str, str):
    plain_mask: str = "1" * 32

    for i in range(m):
        plain_mask = plain_mask.replace("1", "0", 1)

    return plain_mask[0:8], plain_mask[8:16], plain_mask[16:24], plain_mask[24:32]


def get_netaddr(ip: (str, str, str, str), netmask: (str, str, str, str)) -> (str, str, str, str):
    return and_bit_bit(ip[0], netmask[0]), and_bit_bit(ip[1], netmask[1]), and_bit_bit(ip[2], netmask[2]), and_bit_bit(ip[3], netmask[3])


def get_broadcast(ip: (str, str, str, str), wildmask: (str, str, str, str)) -> (str, str, str, str):
    return or_bit_bit(ip[0], wildmask[0]), or_bit_bit(ip[1], wildmask[1]), or_bit_bit(ip[2], wildmask[2]), or_bit_bit(ip[3], wildmask[3])


def server_handler(communication_socket: socket.socket, address: Tuple[str, int]):

    request: bytearray = read_until_terminator_found(communication_socket, b'\n')

    request: str = request.decode("ascii")

    matches = re.findall(r"(\d{1,3}).(\d{1,3}).(\d{1,3}).(\d{1,3})/(\d{1,2})", request)

    print(f"DEBUG: matches -> {matches}")

    if len(matches) != 1:
        print("ERROR: bad request")
        send_bytes(communication_socket, b"ERROR\n")
        return

    match = matches[0]

    ip: (str, str, str, str) = (dec_to_bin(match[0]), dec_to_bin(match[1]), dec_to_bin(match[2]), dec_to_bin(match[3]))
    netmask_raw: int = int(match[4])

    if netmask_raw > 32 or netmask_raw < 0:
        print("ERROR: bad netmask")
        send_bytes(communication_socket, b"ERROR\n")
        return

    netmask: (str, str, str, str) = build_netmask(netmask_raw)
    wildmask: (str, str, str, str) = build_wildmask(netmask_raw)

    netaddr: (str, str, str, str) = get_netaddr(ip, netmask)
    broadcast: (str, str, str, str) = get_broadcast(ip, wildmask)

    print(f"INFO: netmask -> {from_ip_bin_to_ip_dec(netmask)}")
    print(f"INFO: wildmask -> {from_ip_bin_to_ip_dec(wildmask)}")
    print(f"INFO: netaddr -> {from_ip_bin_to_ip_dec(netaddr)}")
    print(f"INFO: broadcast -> {from_ip_bin_to_ip_dec(broadcast)}")

    response: str = f"{from_ip_bin_to_ip_dec(netmask)}\n{from_ip_bin_to_ip_dec(wildmask)}\n{from_ip_bin_to_ip_dec(netaddr)}\n{from_ip_bin_to_ip_dec(broadcast)}\n"

    send_bytes(communication_socket, response.encode("ascii"))

    print(f"INFO: sent response '{response}'")


if __name__ == '__main__':

    # args = parse_args(1)

    port = 12345 # args[1]

    sts = MultiprocessServer('127.0.0.1', port, handler=server_handler)

    sts.start()
