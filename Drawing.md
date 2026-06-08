# NDC — Normalized Device Coordinates

Conosci il piano cartesiano classico — origine al centro, assi che vanno all'infinito. NDC è la stessa cosa ma con una regola: **i valori sono bloccati tra -1 e +1**.

```
          Y = +1
            |
            |
X = -1 ──────────── X = +1
            |
            |
          Y = -1
```

Tutto quello che disegni deve stare dentro questo quadrato. Se metti un vertice a `(2.0, 0.0)` — fuori dallo schermo, non lo vedi.

---

## Confronto con il piano cartesiano che conosci

```
Piano cartesiano normale:       NDC:

  Y                              Y
  ↑                              ↑  +1
4 |                              |
3 |                              |
2 |   * (2, 2)            -1 ────+──── +1  →X
1 |                              |
0 └────────→ X                   |
  0  1  2  3  4                  -1
```

Stessa idea, range diverso. L'origine è sempre al centro.

---

## Il problema pratico

Una finestra 800×600 ha pixel reali. NDC non lavora in pixel — lavora in proporzioni. Quindi:

```
Pixel (0,0)     → NDC (-1, -1)   angolo in basso a sinistra
Pixel (800,600) → NDC (+1, +1)   angolo in alto a destra
Pixel (400,300) → NDC (0, 0)     centro
```

Se vuoi disegnare un cerchio al pixel `(200, 150)` devi fare la conversione a mano ogni volta. Scomodo.

---

## La soluzione: proiezione ortografica

`glm::ortho()` crea una matrice che fa la conversione automaticamente. Tu lavori in pixel, lei converte in NDC prima di mandare i dati alla GPU:

```cpp
glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f)
//         └─────────────────────────────────────────┘
//         "considera X da 0 a 800, Y da 0 a 600"
```

Dopo questa riga puoi posizionare gli oggetti in pixel come faresti in qualsiasi motore 2D, senza pensare mai più all'NDC.

---

# Glossario

## VBO — Vertex Buffer Object

**Buffer** = blocco di memoria. **Object** = OpenGL lo tratta come un oggetto identificato da un ID intero.

È semplicemente **memoria sulla GPU** dove carichi i tuoi dati grezzi. Potrebbe contenere qualsiasi cosa — posizioni, colori, coordinate texture. La GPU da sola non sa cosa significano quei byte.

```
CPU: float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.0, 0.5 }
          |
          | glBufferData()
          ↓
GPU: VBO [ 0xBF000000 0xBF000000 0x3F000000 0xBF000000 ... ]
          (gli stessi float, in memoria della scheda grafica)
```

---

## VAO — Vertex Array Object

Dice alla GPU **come interpretare** i byte del VBO. Risponde a domande tipo:
- Ogni vertice è fatto da quanti float? (2? 3?)
- Da dove inizia il primo vertice?
- I dati di posizione sono all'offset 0? All'offset 12?

Memorizza queste "istruzioni di lettura" in modo che non le devi rispiegare ogni frame.

```
VAO: "ogni vertice = 2 float, inizia all'offset 0, stride 8 byte"
      ↓
VBO: [ -0.5 | -0.5 | 0.5 | -0.5 | 0.0 | 0.5 ]
      └─v0──┘ └────v1───┘ └────v2────┘
```

## L'analogia più semplice

Immagina un file CSV:

```
-0.5,-0.5
 0.5,-0.5
 0.0, 0.5
```

- Il **VBO** è il file CSV (i dati grezzi)
- Il **VAO** è l'intestazione che dice *"la prima colonna è X, la seconda è Y, separatore è la virgola"*

Senza l'intestazione il file è illeggibile. Senza il VAO il VBO è solo byte senza senso.

---

# Come OpenGL disegna un triangolo

## Step 1 — Definisci i vertici sulla CPU

Nel codice C++ si scrive un array di float con le coordinate:

```cpp
float vertices[] = {
    -0.5f, -0.5f,  // vertice in basso a sinistra
     0.5f, -0.5f,  // vertice in basso a destra
     0.0f,  0.5f   // vertice in alto al centro
};
```

Questi numeri sono in **NDC** (da -1 a +1 su entrambi gli assi).
La proiezione ortografica permette di lavorare in pixel invece di NDC.

---

## Step 2 — Carica i vertici sulla GPU (VBO)

La GPU ha la sua memoria separata dalla RAM. I vertici vanno **copiati** lì dentro:

```
RAM (CPU)                     VRAM (GPU)
[ -0.5, -0.5, 0.5, -0.5... ] ──────────────────→ [ VBO ]
                    glBufferData()
```

Il **VBO (Vertex Buffer Object)** è un blocco di memoria sulla GPU.
OpenGL lo identifica con un intero (ID).

---

## Step 3 — Spiega alla GPU come leggere quei dati (VAO)

La GPU vede il VBO come un array grezzo di byte. Non sa che ogni vertice
è fatto da 2 float. Va specificato:

```
VBO: [ -0.5 | -0.5 | 0.5 | -0.5 | 0.0 | 0.5 ]
       └──────┘ └─────────┘ └──────────┘
       vertice 0  vertice 1   vertice 2
       (2 float)  (2 float)   (2 float)
```

Il **VAO (Vertex Array Object)** memorizza questa "ricetta di lettura".
Ogni volta che si vuole disegnare, si binda il VAO e la GPU sa già come
interpretare i dati nel VBO.

---

## Step 4 — Vertex Shader (gira sulla GPU, una volta per vertice)

La GPU prende ogni vertice dal VBO e lancia il **vertex shader**,
un piccolo programma scritto in GLSL:

```glsl
in vec2 aPos;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
```

Viene eseguito **3 volte** (una per vertice).
L'output è la posizione finale del vertice sullo schermo.

---

## Step 5 — Rasterizzazione (automatica, non si scrive)

OpenGL prende i 3 vertici posizionati e determina quali pixel dello
schermo sono **dentro** il triangolo. Lo fa da solo.

```
Vertici posizionati:        Pixel riempiti:
    *                          *
   * *          →            * * *
  * * *                    * * * * *
```

---

## Step 6 — Fragment Shader (gira sulla GPU, una volta per pixel)

Per ogni pixel dentro il triangolo, OpenGL lancia il **fragment shader**:

```glsl
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);  // R, G, B, Alpha → arancione
}
```

Se il triangolo copre 1000 pixel, questo shader gira 1000 volte in parallelo.

---

## Il flusso completo

```
C++ (CPU)                           GPU
──────────────────────────────────────────────────────
1. array float[]          →  VBO (memoria grezza)
2. glVertexAttribPointer  →  VAO (come leggere il VBO)
3. glDrawArrays()         →  lancia la pipeline:
                                ↓
                           Vertex Shader  (×3, uno per vertice)
                                ↓
                           Rasterizzazione (automatica)
                                ↓
                           Fragment Shader (×N, uno per pixel)
                                ↓
                           Framebuffer → schermo
```

---

## Ruolo di ogni componente nel codice

| Componente | Cosa fa |
|---|---|
| `VBO` | Dove risiedono i dati dei vertici sulla GPU |
| `VAO` | Come sono strutturati quei dati |
| `Shader` class | Compila ed esegue i programmi GLSL |
| `glDrawArrays()` | Lancia la pipeline e disegna |
