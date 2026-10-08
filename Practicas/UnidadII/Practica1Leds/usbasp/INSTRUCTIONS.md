# Instrucciones de Uso Programador USBasp

## Revisar la configuración de los fusibles
```bash
avrdude -c usbasp -p m328p -U lfuse:r:-:h -U hfuse:r:-:h
```

## Configurar fusibles para 16 MHz
```bash
    avrdude -c usbasp -p m328p -U lfuse:w:0xFF:m
```

## Cargar programa
```bash
avrdude -c usbasp -p m328p -U flash:w:main.hex:i # 3
```
## Compilar y generar el archivo ejecutable 
```bash
avr-gcc -Wall -Os -mmcu=atmega328p main.S -o main.elf #1
```
## extraemos el archivo compilado de .elf para que 
## el programa lo pueda lo
## pueda leer 
```bash
avr-objcopy -O ihex -R .eeprom main.elf main.hex #  2 
```