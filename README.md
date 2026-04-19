# watchy

## build

```sh
mkdir bin
cd bin
cmake ../
make
```

Build artefacts will be in `./bin/` directory.

## dependencies

- spdlog
- yaml-cpp

## configuration

```yaml
entries:
  - path: "/path/to/dir1"
    command: "git add ."
  - path: "/path/to/dir2"
    command: "git add . && git commit -m \"autocommit\" && git push"
```

