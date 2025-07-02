flowchart LR
    A[Inicio] --> B[Esperar botón de inicio]
    B -- Botón presionado --> C[Iniciar programa]
    C --> D[Contador de giros < 12?]
    D -- No --> E[Avanzar 1.5s y detenerse]
    E --> F[Fin]
    D -- Sí --> G[Leer sensores ultrasónicos]
    G --> H{¿Frontal > 20cm?}
    H -- Sí --> I[Avanzar recto]
    I --> J{¿En tiempo de espera tras giro?}
    J -- Sí --> G
    J -- No --> K{¿Izquierda > 200cm?}
    K -- Sí --> L[Girar izquierda, +1 giro, reset tiempo]
    L --> I
    K -- No --> M{¿Derecha > 200cm?}
    M -- Sí --> N[Girar derecha, +1 giro, reset tiempo]
    N --> I
    M -- No --> I