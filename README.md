# Afficheur à persistance rétinienne

## Construction

```bash
mkdir build && cd build
cmake ..
make
```

### Specify AVR-GCC build

You can specify a custom AVR-GCC BUILD at configuration:

```bash
cd build
rm -rf *
cmake .. -DTOOLCHAIN_CUSTOM_AVR_GCC_PATH=<your/avr-gcc/path>
make
```

## flash

```bash
make flash-<target_name>
```
