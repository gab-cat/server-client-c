# server-client-c

## Battle Ships Game

### Overview

This repository contains a simple client-server application for message exchange and a battle ships game implementation. The battle ships game allows two players to connect and play against each other over a network.

### Files

- `202200598_LE10_client.c`: A simple client application for message exchange.
- `202200598_LE10_server.c`: A simple server application for message exchange.
- `battle_ships.c`: The battle ships game implementation.

### How to Run the Battle Ships Game

1. Compile the `battle_ships.c` file:
   ```sh
   gcc -o battle_ships battle_ships.c
   ```

2. Run the `battle_ships` executable:
   ```sh
   ./battle_ships
   ```

3. Choose the mode:
   - Enter `1` for server mode.
   - Enter `2` for client mode.

4. Follow the prompts to establish a connection:
   - For server mode, enter the port number to listen on.
   - For client mode, enter the hostname and port number of the server to connect to.

5. Once connected, the game will begin. Players take turns to guess the location of the opponent's ships on a 5x5 board.

6. To exit the game, type `exit` and press Enter.

### Game Mechanics

- The game board is a 5x5 grid represented by `~` for water.
- Players take turns to guess the location of the opponent's ships.
- The game continues until one player decides to exit by typing `exit`.

### Example

#### Server
```sh
./battle_ships 12345
```

#### Client
```sh
./battle_ships localhost 12345
```

### Notes

- Ensure that the server is started before the client attempts to connect.
- The game currently does not include ship placement or hit/miss detection. These features can be added for a more complete implementation.
