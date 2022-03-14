# netcopy

UNIX POSIX Socket communication tool

## Usage

```shell
netcopy DEST_IP DEST_PORT [OPTIONS]

```

Simple loopback tcp connection

```
netcopy 127.0.0.1 55555 < ./test.me
```

### Options

- `-u` for UDP
- `-t` for TCP (is default)
- `-p PORT` set self port
- `-v` active verbose print in stdout
- `-4` uses IPv4 (default)
- `-6` uses IPv6

```shell
netcopy DEST_IP DEST_PORT -u -6 -p 4273
```

