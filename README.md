# PROGETTO ELEMENTI DI BIOINFORMATICA
## Long Bitap

Implementazione dell'algortimo _bitap_ (detto anche _shift-and_) per pattern
più lunghi di una word della cpu.
In input si hanno due file, uno per il pattern e uno per il testo.
Nella cartella slow è presente una seconda versione dell'algoritmo generalmente più
lenta ma comoda nel caso in cui le occorrenze siano in altissima numerosità.

### USAGE

- compilazione:
```
make
```

- esecuzione:
```
./bitaplong.out --pattern <pattern> --text <text>
```
oppure:
```
./bitaplong.out -p <pattern> -t <text>
```
