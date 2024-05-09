# Simple Client Server

Semplici client e server scritti in Python e C per sopravvivere all'esame di reti.

**Non rubare, contribuisci.**

Autore: **Nicola Ricciardi**


## Python

### Client

Per Python è presente un'unica classe `Client` che permette di agganciare velocemente un client ad un server.

`Client` dispone di un attributo `socket` (di tipo `socket.socket`) per accedere alle funzionalità canoniche:

```python
client = Client()

client.connect('127.0.0.1', 12345)      # server information

client.socket.send("hello world".encode("utf-8"))

data = client.socket.recv(1024)
```

### Server

Per Python sono presenti due server:

- `SingleThreadServer`
- `MultiThreadServer`

Per entrambi deve essere specificato indirizzo IP e porta del socket. Inoltre deve essere specificato un **handler** per gestire il comportamento del server alla ricezione dei dati (del client).

Ad esempio, per leggere i dati del client e re-inviarli al client stesso:

```python
def server_handler(communication_socket: socket.socket, address: str):

    data = communication_socket.recv(1024)

    print(f"{data}")

    communication_socket.send(data)


if __name__ == '__main__':
    mts = MultithreadServer('127.0.0.1', 12345, handler=server_handler)

    mts.start()
```
