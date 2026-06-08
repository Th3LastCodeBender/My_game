# OpenGL come Global State Machine

## Cos'è una State Machine

Una state machine è un sistema che ha uno **stato interno** e si comporta diversamente in base a quello stato. L'output dipende non solo dall'input, ma da *cosa è stato configurato prima*.

Esempio banale — un semaforo:
```
stato = ROSSO  → le macchine si fermano
stato = VERDE  → le macchine passano

input: "è arrivata una macchina"
output: dipende dallo STATO, non solo dall'input
```

---

## Perché la GPU è una State Machine

Pensa all'architettura Von Neumann: CPU legge istruzioni dalla RAM, le esegue, scrive risultati. La GPU funziona diversamente — è un processore **massicciamente parallelo** con migliaia di core piccoli, progettato per fare la stessa operazione su milioni di dati contemporaneamente.

Il problema è: come comunichi con una GPU da un programma CPU?

Negli anni '90 la risposta fu: **un driver con stato globale**. Invece di creare oggetti e passarli in giro, tu *configuri il driver* e poi dici "vai". Il driver mantiene tutto lo stato e sa cosa fare.

```
CPU: "binda questo buffer"         → stato driver: buffer_attivo = X
CPU: "spiega come leggere i dati"  → stato driver: formato = 2 float per vertice
CPU: "disegna 3 vertici"           → GPU legge lo stato e sa esattamente cosa fare
```

---

## Perché OpenGL è una Global State Machine

OpenGL non è un oggetto — è un'**API che parla con il driver della GPU**. Il driver vive in un processo separato (o nel kernel), mantiene il suo stato interno, e tu lo manipoli tramite chiamate `gl*()`.

Quando chiami `glBindBuffer(GL_ARRAY_BUFFER, vbo)` non stai passando un parametro a una funzione — stai **cambiando una variabile globale nel driver**:

```
driver_state.array_buffer_attivo = vbo;
```

Tutte le chiamate successive che riguardano `GL_ARRAY_BUFFER` opereranno su quel VBO finché non lo cambi. Esattamente come il semaforo — il comportamento dipende dallo stato corrente.

---

## Perché hanno scelto questo approccio

### 1 — Anni '90: la GPU era un chip fisso

Le prime GPU non erano programmabili. Avevano una pipeline rigida: trasformazione vertici → texture → illuminazione → pixel. OpenGL nasceva come interfaccia a quella pipeline fissa. Aveva senso avere uno stato globale perché stavi configurando **hardware fisico**.

```
[OpenGL 1.x]
glEnable(GL_LIGHTING)      → accendi il modulo hardware illuminazione
glMaterialfv(GL_DIFFUSE)   → configura il materiale
glDrawArrays(...)          → hardware esegue la pipeline fissa
```

### 2 — Semplicità per il driver

Il driver non deve gestire oggetti complessi. Mantiene una struct con lo stato corrente e risponde a comandi. Facile da implementare su hardware diverso.

### 3 — Performance degli anni '90

Passare oggetti tra CPU e GPU aveva overhead enorme sul BUS. Uno stato globale nel driver minimizzava il traffico.

---

## I problemi che questo approccio ha creato

**Problema 1 — Ordine delle chiamate critico**
```cpp
glBindBuffer(GL_ARRAY_BUFFER, vbo);      // step 1
glVertexAttribPointer(...);              // step 2 — DEVE venire dopo step 1
glEnableVertexAttribArray(0);            // step 3 — DEVE venire dopo step 2
```
Se sbagli l'ordine → comportamento undefined, nessun errore chiaro.

**Problema 2 — Thread**
La state machine è globale per thread. Se hai due thread che chiamano `gl*()` contemporaneamente → race condition sul driver. Per questo `glfwMakeContextCurrent()` esiste — lega il contesto a un thread specifico.

**Problema 3 — Stato implicito**
Se dimentichi di fare `glBindVertexArray(0)` dopo aver configurato un VAO, il VAO successivo che crei potrebbe ricevere configurazioni che non volevi. Bug difficilissimi da trovare.

---

## Perché OpenGL 3.3 Core mantiene questo approccio

Non lo cambia, ma lo **semplifica**. Il Core Profile rimuove tutta la pipeline fissa degli anni '90 (quella con `glBegin/glEnd`, `glVertex3f` ecc.) e ti obbliga a usare shader. Il risultato è uno stato globale più piccolo e prevedibile.

Le alternative moderne che risolvono questi problemi sono:
- **Vulkan** (2016) — niente state machine globale, tutto esplicito, oggetti veri, multi-thread nativo. Ma è 10 volte più complesso.
- **Metal** (Apple), **DirectX 12** (Microsoft) — stessa filosofia di Vulkan.

OpenGL rimane la scelta didattica perché il suo stato globale, pur essendo un limite architetturale, ti permette di disegnare qualcosa a schermo in 50 righe invece di 500.

---

## Vantaggi e svantaggi

| 									| State Machine (OpenGL) | Esplicito (Vulkan) |
|---|---|---|
| Curva di apprendimento 			| Bassa | Altissima |
| Ordine chiamate					| Critico e implicito | Esplicito e verificato |
| Multi-thread 						| Difficile | Nativo |
| Debug errori 						| Spesso silenzioso | Validation layer espliciti |
| Righe per disegnare un triangolo	| ~50 | ~500 |
