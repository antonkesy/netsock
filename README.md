# netsock

UNIX POSIX UDP/TCP Socket communication tool

Cheap copy of `netcat`

## Build

```shell
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

```shell
netsock IP PORT [OPTIONS]
```

Simple loopback tcp connection

```
netsock 127.0.0.1 55555 > ./out &
netsock 127.0.0.1 55555 < ./in
```

### Options

- `-l` for listener
- `-u` for UDP
- `-t` for TCP (default)
- `-v` activate verbose print in stdout
- `-4` force IPv4 (default select by parsing ip address (first v4))
- `-6` force IPv6

When IP-Version is set, use `localhost` for loopback address

#### Example

IPv6 TCP Listener

TCP is default and doesn't need an extra option

```shell
netsock SELF_IP SELF_PORT -6 -l
```

IPv4 UDP Sender

IPv4 is default -> no flag required

```shell
netsock DEST_IP DEST_PORT -u
```

## Test

Run `./test/test.sh` to confirm functionality with basic usage tests.  
GTests are also available by running `ctest` inside of `build`.
