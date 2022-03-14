# netcopy

UNIX POSIX UDP/TCP Socket communication tool

## Usage

```shell
netcopy IP PORT [OPTIONS]
```

Simple loopback tcp connection

```
netcopy 127.0.0.1 55555 > ./out &
netcopy 127.0.0.1 55555 < ./in 
```

### Options

- `-l` for listener
- `-u` for UDP
- `-t` for TCP (default)
- `-v` activate verbose print in stdout
- `-4` force IPv4 (default)
- `-6` force IPv6

#### Example

IPv6 TCP Listener

TCP is default and doesn't need an extra option

```shell
netcopy SELF_IP SELF_PORT -6 -l
```

IPv4 UDP Sender

IPv4 is default -> no flag required

```shell
netcopy DEST_IP DEST_PORT -u
```

## Test

run ```./test/test.sh``` to confirm functionality with basic usage tests