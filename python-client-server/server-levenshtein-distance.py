import numpy
import socket

from server.singlethread_server import SingleThreadServer
from server.multithread_server import MultithreadServer
from server.multiprocess_fork_server import MultiprocessForkServer
from server.multiprocess_server import MultiprocessServer
from common.socket_utils import read_until_terminator_found, send_bytes
from common.cli import parse_args
import re


def print_distances(distances, token1Length, token2Length):
    for t1 in range(token1Length + 1):
        for t2 in range(token2Length + 1):
            print(int(distances[t1][t2]), end=" ")
        print()

def levenshteinDistanceDP(token1, token2):
    distances = numpy.zeros((len(token1) + 1, len(token2) + 1))

    for t1 in range(len(token1) + 1):
        distances[t1][0] = t1

    for t2 in range(len(token2) + 1):
        distances[0][t2] = t2

    a = 0
    b = 0
    c = 0

    for t1 in range(1, len(token1) + 1):
        for t2 in range(1, len(token2) + 1):
            if (token1[t1 - 1] == token2[t2 - 1]):
                distances[t1][t2] = distances[t1 - 1][t2 - 1]
            else:
                a = distances[t1][t2 - 1]
                b = distances[t1 - 1][t2]
                c = distances[t1 - 1][t2 - 1]

                if (a <= b and a <= c):
                    distances[t1][t2] = a + 1
                elif (b <= a and b <= c):
                    distances[t1][t2] = b + 1
                else:
                    distances[t1][t2] = c + 1

    print_distances(distances, len(token1), len(token2))
    return distances[len(token1)][len(token2)]


def custom_ld(str1: str, str2: str) -> int:

    m = len(str1)
    n = len(str2)

    d = []

    for i in range(m + 1):

        row = [-1 for i in range(n + 1)]

        d.append(row)

    for i in range(m+1):

        d[i][0] = i

    for j in range(n+1):
        d[0][j] = j

    for i in range(1, m+1):
        for j in range(1, n+1):
            if str1[i - 1] != str2[j - 1]:
                k = min(d[i][j - 1], d[i - 1][j], d[i - 1][j - 1])

                d[i][j] = k + 1

            else:
                d[i][j] = d[i - 1][j - 1]

    distance = d[m][n]

    return int(distance)

def server_handler(communication_socket: socket.socket, address: str):

    request: bytearray = read_until_terminator_found(communication_socket, b'\n')

    request: str = request.decode("ascii")

    matches = re.findall(r"(.{1,50}) (.{1,50})", request)

    if len(matches) != 1:
        print("ERROR: bad request")
        send_bytes(communication_socket, f"Errore: input errato".encode("ascii"))
        return

    body: str = matches[0]

    print(f"INFO: request body: {body}")

    str1: str = body[0]
    str2: str = body[1]

    n = int(custom_ld(str1, str2))

    print(f"INFO: n -> {n}")

    response: str = f'La distanza di Levenshtein tra le stringhe "{str1}" e "{str2}" e\' pari a {n}.\n'

    send_bytes(communication_socket, response.encode("ascii"))

    print(f"INFO: sent response '{response}'")


if __name__ == '__main__':

    # args = parse_args(1)

    port = 12345 # args[1]

    sts = MultiprocessServer('127.0.0.1', port, handler=server_handler)

    sts.start()


