import board
import busio
import digitalio
import pwmio
import time
from adafruit_motor import servo
from adafruit_bno08x import BNO08X_I2C

# --- Configuración de pines ---
# Ultrasonidos
trigger_front = digitalio.DigitalInOut(board.GP2)
trigger_front.direction = digitalio.Direction.OUTPUT
echo_front = digitalio.DigitalInOut(board.GP3)
echo_front.direction = digitalio.Direction.INPUT

trigger_left = digitalio.DigitalInOut(board.GP6)
trigger_left.direction = digitalio.Direction.OUTPUT
echo_left = digitalio.DigitalInOut(board.GP7)
echo_left.direction = digitalio.Direction.INPUT

trigger_right = digitalio.DigitalInOut(board.GP8)
trigger_right.direction = digitalio.Direction.OUTPUT
echo_right = digitalio.DigitalInOut(board.GP9)
echo_right.direction = digitalio.Direction.INPUT

# Servo direccional
pwm_servo = pwmio.PWMOut(board.GP17, frequency=50)
myservo = servo.Servo(pwm_servo, min_pulse=500, max_pulse=2500)

# ESC motor
pwm_esc = pwmio.PWMOut(board.GP16, frequency=50)

# LED integrado (GP25 en Pico)
led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT

# I2C para BNO08x
i2c = busio.I2C(board.GP5, board.GP4)  # SCL, SDA

bno = BNO08X_I2C(i2c)

# --- Constantes ---
DISTANCIA_OBSTACULO_FRONTAL = 15
DISTANCIA_OBSTACULO_LATERAL = 50
SERVO_MIN = 30
SERVO_MAX = 150
SERVO_CENTRO = 90  # Ajustado a 90º para servo en CircuitPython

# Variables de estado
motor_en_marcha = False
contador_vueltas = 0
angulo_acumulado_z = 0.0
robot_detenido = False
tiempo_anterior = time.monotonic()

# --- Funciones ---

def ping(trigger, echo):
    """Mide distancia con sensor ultrasónico HC-SR04."""
    trigger.value = False
    time.sleep(0.000002)
    trigger.value = True
    time.sleep(0.00001)
    trigger.value = False

    start_time = time.monotonic()
    timeout = start_time + 0.02  # 20 ms timeout

    while not echo.value:
        if time.monotonic() > timeout:
            return None
    start = time.monotonic()

    while echo.value:
        if time.monotonic() > timeout:
            return None
    end = time.monotonic()

    duration = end - start
    distance_cm = (duration * 34300) / 2
    return distance_cm

def esc_write(angle):
    """Controla el ESC con ángulo entre 0 y 180 (mapeado a pulso PWM)."""
    min_pulse = 1000  # us
    max_pulse = 2000  # us
    pulse_width = min_pulse + (angle / 180) * (max_pulse - min_pulse)
    duty_cycle = int(pulse_width * 65535 / 20000)
    pwm_esc.duty_cycle = duty_cycle

def servo_write(angle):
    """Controla el servo direccional (0-180 grados)."""
    # Limitar ángulo para evitar movimientos extremos
    angle = max(min(angle, SERVO_MAX), SERVO_MIN)
    myservo.angle = angle

def corregir_direccion(angulo_z):
    umbral = 2.0
    max_correccion = 20
    if abs(angulo_z) < umbral:
        servo_write(SERVO_CENTRO)
    else:
        angulo_servo = SERVO_CENTRO - (angulo_z * (max_correccion / 10.0))
        angulo_servo = max(min(angulo_servo, SERVO_MAX), SERVO_MIN)
        servo_write(angulo_servo)

def adelante():
    global motor_en_marcha
    # Armar ESC
    esc_write(90)
    time.sleep(0.3)
    # Aumentar velocidad gradualmente
    for speed in range(90, 111, 1):
        esc_write(speed)
        time.sleep(0.25)
    motor_en_marcha = True

def atras():
    global motor_en_marcha
    esc_write(90)
    time.sleep(0.3)
    for speed in range(90, 29, -10):
        esc_write(speed)
        time.sleep(0.25)
    motor_en_marcha = True

def parar():
    global motor_en_marcha
    esc_write(90)
    motor_en_marcha = False

def izquierda():
    esc_write(110)
    for ang in range(SERVO_CENTRO, SERVO_MAX + 1, 2):
        servo_write(ang)
        time.sleep(0.015)
    time.sleep(0.45)
    servo_write(SERVO_CENTRO)
    esc_write(110)

def derecha():
    esc_write(110)
    for ang in range(SERVO_CENTRO, SERVO_MIN - 1, -2):
        servo_write(ang)
        time.sleep(0.015)
    time.sleep(0.45)
    servo_write(SERVO_CENTRO)
    esc_write(110)

def doce_vueltas():
    global angulo_acumulado_z, contador_vueltas, robot_detenido, tiempo_anterior

    if robot_detenido:
        return

    led.value = True

    frontal = ping(trigger_front, echo_front) or 999
    izquierda = ping(trigger_left, echo_left) or 999
    derecha = ping(trigger_right, echo_right) or 999

    tiempo_actual = time.monotonic()
    delta_t = tiempo_actual - tiempo_anterior
    tiempo_anterior = tiempo_actual

    # Leer giroscopio BNO08x
    gyro_z_dps = 0.0
    for event in bno.events():
        if event.sensor_id == bno.SENSOR_GYROSCOPE_CALIBRATED:
            gyro_z_dps = event.gyroscope.z * 57.2958  # rad/s a grados/s

    angulo_acumulado_z += gyro_z_dps * delta_t

    # Detectar giro completo (±89°)
    if abs(angulo_acumulado_z) > 89:
        contador_vueltas += 1
        if angulo_acumulado_z > 0:
            print("Giro a la derecha detectado.")
        else:
            print("Giro a la izquierda detectado.")
        angulo_acumulado_z = 0
        print(f"Giros: {contador_vueltas}")

    print(f"Frontal: {frontal:.1f} cm | Izq: {izquierda:.1f} cm | Der: {derecha:.1f} cm | Ángulo Z: {angulo_acumulado_z:.2f}")

    if frontal > DISTANCIA_OBSTACULO_FRONTAL:
        if not motor_en_marcha:
            adelante()
        corregir_direccion(angulo_acumulado_z)
    else:
        if motor_en_marcha:
            parar()

        if izquierda > DISTANCIA_OBSTACULO_LATERAL and izquierda > derecha:
            izquierda()
        elif derecha > DISTANCIA_OBSTACULO_LATERAL and derecha > izquierda:
            derecha()
        else:
            atras()
            time.sleep(0.5)
            parar()
        time.sleep(1)
        servo_write(SERVO_CENTRO)

    if contador_vueltas >= 12:
        adelante()
        time.sleep(2.5)
        parar()
        print("¡Completados 12 giros!")
        robot_detenido = True

    led.value = False

# --- Programa principal ---

# Armar ESC al iniciar
esc_write(90)
time.sleep(3)

print("Inicio del programa")

while True:
    doce_vueltas()