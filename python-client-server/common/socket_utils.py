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

        if terminator_found:
            if not include_terminator:
                result = result[0:len(result) - len(terminator)]

            break

    return result


def send_bytes(socketd: socket.socket, bytes_to_send: bytes | bytearray) -> None:

    socketd.sendall(bytes_to_send)


def send_file(socketd: socket.socket, filepath: str, terminator: bytes = b'\x00') -> None:

    with open(filepath, "rb") as file:
        lines: list[bytes] = file.readlines()

        for line in lines:
            line_in_bytes = line

            send_bytes(socketd, line_in_bytes)

        send_bytes(socketd, terminator)


def receive_file(socketd: socket.socket, filepath: str, terminator: bytes = b'\x00', include_terminator: bool = False) -> None:

    with open(filepath, "wb") as file:

        last_n_bytes_read = bytearray()
        while True:
            buffer: bytes = socketd.recv(1)     # MUST BE 1

            last_n_bytes_read.extend(buffer)

            if len(last_n_bytes_read) > len(terminator):
                last_n_bytes_read.pop(0)

            terminator_found = last_n_bytes_read == terminator

            if terminator_found and not include_terminator:
                break

            file.write(buffer)

            if terminator_found:
                break

