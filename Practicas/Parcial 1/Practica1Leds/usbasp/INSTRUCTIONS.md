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
avrdude -c usbasp -p m328p -U flash:w:main.hex:i
```
