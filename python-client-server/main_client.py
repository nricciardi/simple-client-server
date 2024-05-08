from client.client import Client


if __name__ == '__main__':
    client = Client()

    client.connect('127.0.0.1', 12345)

    client.socket.send("ciao".encode("utf-8"))

    data = client.socket.recv(1024)

    print(f"{data}")
