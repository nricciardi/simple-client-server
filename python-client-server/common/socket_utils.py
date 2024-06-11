import socket


def read_until_terminator_found(socketd: socket.socket, terminator: bytes, include_terminator: bool = False) -> bytearray:

    result = bytearray()

    buffer_size = 1

    while True:
        buffer: bytes = socketd.recv(buffer_size)

        if len(buffer) == 0:
            print("INFO: client disconnected")
            break

        result.extend(buffer)

        terminator_found = result[-len(terminator):] == terminator

        if terminator_found and not include_terminator:
            break

        if terminator_found:
            break

    return result


def send_bytes(socketd: socket.socket, bytes_to_send: bytes | bytearray) -> None:

    socketd.sendall(bytes_to_send)


def send_file(socketd: socket.socket, filepath: str) -> None:

    with open(filepath, "rb") as file:
        lines: list[bytes] = file.readlines()

        for line in lines:
            line_in_bytes = line

            send_bytes(socketd, line_in_bytes)


def receive_file(socketd: socket.socket, filepath: str) -> None:

    with open(filepath, "wb") as file:

        while True:
            buffer: bytes = socketd.recv(1)

            file.write(buffer)

            if len(buffer) == 0:
                break
