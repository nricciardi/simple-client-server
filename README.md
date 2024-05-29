# Simple Client Server

Semplici client e server scritti in Python e C per sopravvivere all'esame di reti.

**Non rubare, contribuisci.**

Autore: **Nicola Ricciardi**

TODO:

- Esempio download/upload

## C

### Client

Utilizzare la funzione:

```c
int connect_to_server(int domain, int type, int protocol, const struct sockaddr* server_address, socklen_t server_address_length)
```

Oppure la più specifica:

```c
int connect_to_tcp_ipv4_server(const struct sockaddr_in* server_address)
```

Ad esempio:

```c
#include "client.h"

int main(void) {

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);

    inet_aton("127.0.0.1", &server_address.sin_addr);

    int socketd = connect_to_tcp_ipv4_server(&server_address);

    char* hello = "hello";
    send(socketd, hello, sizeof(hello), 0);

    return 0;
}
```

### Server

Includere:

```c
#include "server.h"
```

1. Ottenere un nuovo socket descriptor di un server mediante la funzione generale o utilizzare quella predisposta per i server TCP IPv4

```c
struct sockaddr_in addr = build_ipv4_sockaddr(address, port);

int socketd = new_server(AF_INET, SOCK_STREAM, 0, (struct sockaddr*) &addr, sizeof(addr));

// oppure

int socketd = new_tcp_ipv4_server("127.0.0.1", 12345);
```

2. Supponendo di andare ad utilizzare un server IPv4, implementare:

```c
void on_ipv4_server_start(int communication_socketd, struct sockaddr_in client_address) {
    // ...
}
```

3. Avviare il server

```c
start_singlethread_server(socketd, 25);

// oppure

start_multiprocess_server(socketd, 42);
```


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

# Contributori

- Nicola Ricciardi
- Filippo Garagnani