# Control de brazo robótico con potenciómetros (ESP32-S3 + Python + PyBullet)

Proyecto de control de un brazo robótico en tiempo real utilizando un ESP32-S3 conectado a dos potenciómetros. Los valores analógicos obtenidos por el ESP32-S3 son enviados mediante comunicación serie a Python, donde son procesados y utilizados para controlar dos articulaciones de un brazo robótico simulado en PyBullet.

## Demostración

**Video de funcionamiento:**

[![Demo del proyecto](docs/MINIATURA.png)](docs/ACTIVIDAD4_VIDEO.mp4)

> Si el video no se reproduce directamente en GitHub, descárgalo desde [`docs/ACTIVIDAD4_VIDEO.mp4`](docs/ACTIVIDAD4_VIDEO.mp4).

**Simulación del brazo robótico:**

![Brazo robótico en PyBullet](docs/brazo_pybullet.png)

**Montaje físico:**

![Montaje de los potenciómetros](docs/montaje.png)

## Funcionalidades

| Potenciómetro | Comunicación | Acción en el brazo robótico |
|---|---|---|
| Potenciómetro 1 | ADC 0–4095 | Giro de la base |
| Potenciómetro 2 | ADC 0–4095 | Movimiento de la articulación del brazo |

El sistema permite controlar dos grados de libertad del brazo robótico mediante dos potenciómetros conectados al ESP32-S3.

El primer potenciómetro controla el movimiento lateral de la base.

El segundo potenciómetro controla el movimiento de la articulación principal del brazo.

## Funcionamiento general

```text
Potenciómetros
      ↓
ESP32-S3 (ADC)
      ↓
Serial (115200 baud)
      ↓
Python + PySerial
      ↓
Conversión ADC → Ángulo
      ↓
PyBullet
      ↓
Brazo robótico (URDF)
```

1. El ESP32-S3 realiza la lectura de los dos potenciómetros mediante sus entradas analógicas.
2. Cada lectura genera un valor entre 0 y 4095 debido al ADC de 12 bits.
3. Los valores son enviados mediante el puerto serie a 115200 baudios.
4. Python recibe los valores mediante PySerial.
5. Si los datos recibidos contienen dos valores válidos, Python los separa y valida.
6. Los valores ADC son convertidos a posiciones angulares.
7. El primer valor controla la articulación de la base.
8. El segundo valor controla la articulación del brazo.
9. PyBullet actualiza la posición de las articulaciones en tiempo real.

Los datos enviados por el ESP32-S3 tienen el siguiente formato:

```text
pot1,pot2
```

Ejemplo:

```text
2335,3060
```

## Requisitos

### Hardware

- 1 ESP32-S3
- 2 potenciómetros de 10 kΩ
- Protoboard y cables
- Cable USB de datos
- Computador

### Software

- Python 3.9 o superior
- Visual Studio Code
- PlatformIO
- PyBullet
- PySerial
- Arduino Framework
- Archivo URDF del brazo robótico

## Conexiones

| Potenciómetro | Pin ESP32-S3 | Función |
|---|---|---|
| Potenciómetro 1 | GPIO 4 | Control de la base |
| Potenciómetro 2 | GPIO 5 | Control del brazo |

Circuito para cada potenciómetro:

```text
             Potenciómetro
             ┌───────────┐
3.3 V ───────┤            │
             │     ↕      ├──── Señal → GPIO
GND ─────────┤            │
             └───────────┘
```

El potenciómetro funciona como divisor de tensión. Al girarlo, cambia el voltaje de salida que es leído por la entrada analógica del ESP32-S3.

## Estructura del proyecto

```text
.
├── docs/
│   ├── demo.mp4                 # Video de funcionamiento
│   ├── demo-thumbnail.png       # Miniatura del video
│   ├── brazo_pybullet.png       # Captura de la simulación
│   └── montaje.png              # Montaje físico
├── python/
│   ├── main.py                  # Control del brazo mediante PyBullet
│   └── brazo.urdf               # Modelo del brazo robótico
├── esp32/
│   ├── platformio.ini
│   └── src/
│       └── main.cpp             # Firmware del ESP32-S3
└── README.md
```

> Ajusta los nombres de carpetas y archivos según la organización real de tu repositorio.

## Instalación y uso

### 1. Clonar el repositorio

```bash
git clone https://github.com/mafer1608-7/<tu-repositorio>.git
cd <tu-repositorio>
```

### 2. Cargar el firmware al ESP32-S3

1. Abre la carpeta del firmware (`esp32/`) en VS Code con PlatformIO.
2. Verifica que tu `platformio.ini` sea similar a este:

```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
build_flags =
    -DARDUINO_USB_CDC_ON_BOOT=1
    -DARDUINO_USB_MODE=1
upload_port = COM6
monitor_port = COM6
monitor_speed = 115200
```

3. Compila y sube el código mediante el botón **Upload** de PlatformIO.

El firmware realiza la lectura de los dos potenciómetros y envía sus valores mediante comunicación serie.

### 3. Instalar dependencias de Python

```bash
pip install pybullet pyserial
```

### 4. Configurar el puerto serie

En `main.py`, el puerto utilizado por el ESP32-S3 está configurado como `COM6`:

```python
PUERTO = "COM6"
BAUDIOS = 115200
```

- Windows: `COM3`, `COM5`, `COM6`, etc.
- Linux: `/dev/ttyUSB0` o `/dev/ttyACM0`.
- macOS: `/dev/cu.usbserial-XXXX`.

Si el ESP32-S3 aparece en otro puerto, modifica la variable `PUERTO` en `main.py`.

### 5. Ejecutar

Antes de ejecutar Python, cierra el Monitor Serial de PlatformIO para liberar el puerto COM6.

Desde la terminal de VS Code ejecuta:

```bash
python main.py
```

Se abrirá una ventana de PyBullet mostrando el brazo robótico.

Gira los potenciómetros para controlar las articulaciones del brazo.

## Solución de problemas

| Problema | Posible causa o solución |
|---|---|
| `Error al abrir el puerto` | Puerto COM incorrecto o ESP32-S3 desconectado. Verifica el número de puerto. |
| El puerto está ocupado o hay acceso denegado | Cierra el Monitor Serial de PlatformIO; solo un programa puede utilizar el puerto simultáneamente. |
| El brazo no se mueve | Verifica que el ESP32-S3 esté enviando los valores de los potenciómetros y que Python esté utilizando el puerto correcto. |
| Los potenciómetros no responden | Revisa las conexiones de GPIO 4 y GPIO 5, alimentación y GND. |
| `brazo.urdf` no encontrado | Ejecuta `main.py` desde la carpeta donde se encuentra el archivo URDF. |
| Error `No module named pybullet` | Instala PyBullet ejecutando `pip install pybullet`. |
| Error `No module named serial` | Instala PySerial ejecutando `pip install pyserial`. |
| El ESP32-S3 no responde justo al iniciar | Al abrir el puerto serie el ESP32-S3 puede reiniciarse; el `time.sleep(2)` permite que termine de iniciar antes de recibir los datos. |
| PyBullet se cierra inesperadamente | Verifica que el archivo URDF sea válido y que las articulaciones utilizadas correspondan a los índices configurados. |

## Personalización

- Cambiar los pines de los potenciómetros modificando `POT1_PIN` y `POT2_PIN` en `main.cpp`.
- Cambiar el puerto serie modificando `PUERTO` en `main.py`.
- Cambiar la velocidad de comunicación modificando `BAUDIOS`.
- Modificar el rango de movimiento de la base mediante `MIN_BASE` y `MAX_BASE`.
- Modificar el rango de movimiento del brazo mediante `MIN_BRAZO` y `MAX_BRAZO`.
- Cambiar la fuerza utilizada para controlar las articulaciones.
- Modificar el modelo del brazo robótico mediante el archivo `brazo.urdf`.

## Tecnologías utilizadas

- ESP32-S3
- Arduino Framework
- PlatformIO
- Python
- PySerial
- PyBullet
- URDF
- Visual Studio Code

## Licencia

Este proyecto se desarrolla con fines académicos y educativos.

## Autor

Desarrollado por <Mafe Peñuela> — [@mafer1608-7](https://github.com/mafer1608-7)
