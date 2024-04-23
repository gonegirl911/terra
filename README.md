# Terra

Producing an executable out of the project and running it is as easy as pasting
the following in your shell:

```sh
cmake -DCMAKE_BUILD_TYPE=Release -B build \
    && cmake --build build \
    && cd build \
    && ./terra
```
