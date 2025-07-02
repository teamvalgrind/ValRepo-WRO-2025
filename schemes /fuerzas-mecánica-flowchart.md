flowchart LR
    A[Motor] --> B[Diferencial]
    B --> C[Semieje der.\nRueda ext. ωₒ]
    B --> D[Semieje izq.\nRueda int. ωᵢ]
    C --> E[Fuerza de tracción ↑]
    D --> F[Ángulo de giro θᵢ > θₒ]
    E & F --> G[Cuadro]
    G --> H{Deflexión estructural?}
    H -->|Sí| I[Pérdida de Ackermann]
    H -->|No| J[Comportamiento ideal]