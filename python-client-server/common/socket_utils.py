import socket


def read_until_terminator_found(socketd: socket.socket, terminator: bytes, include_terminator: bool = False) -> bytearray:

    result = bytearray()

    buffer_size = len(terminator)

    while True:
        buffer: bytes = socketd.recv(buffer_size)

        if len(buffer) == 0:
            print("INFO: client disconnected")
            break

        terminator_found = buffer == terminator

        if terminator_found and not include_terminator:
            break

        result.extend(buffer)

        if terminator_found:
            break

    return result


def send_bytes(socketd: socket.socket, bytes_to_send: bytes | bytearray) -> None:

    socketd.sendall(bytes_to_send)


def send_file(socketd: socket.socket, filepath: str) -> None:

    with open(filepath, "r") as file:
        lines: list = file.readlines()

        for line in lines:
            line_in_bytes = bytes(line)

            send_bytes(socketd, line_in_bytes)