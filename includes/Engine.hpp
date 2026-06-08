#ifndef STRUCTS_HPP
#	define STRUCTS_HPP

#include "header.hpp"

using namespace glm;

typedef struct s_Engine 
{
	GLFWwindow	*window;
	float		lastTime;

	s_Engine ()
	{
		//REVIEW - Funzione di inizializzazione di OpenGL, non penso di dover spiegare molto a parte il fatto
		// che nel caso in cui non vada a buon fine restituisce GLFW_FALSE ossia 0
		if (glfwInit() == GLFW_FALSE)
		{
			std::cerr << "[structs.hpp->s_Engine]: GLFW initialization failed!";
			exit(EXIT_FAILURE);
		}

		//REVIEW - Indicare le versioni di OpenGL
		// Senza queste specificazioni forzate il driver potrebbe dare una versione vecchia
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // -->Forzo la crezione della finestra alla versione di OpenGL che voglio io
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // -->Stessa cosa della riga sopra, è come definire min e max
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // -->Forzo la scelta del Core

		window = glfwCreateWindow(WIDTH, HEIGHT, "my gayme", nullptr, nullptr);
		if (!window)
		{
			std::cerr << "[structs.hpp->s_Engine]: glfw Window failed!";
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		//REVIEW - OpenGL contexts and state machine init
		// A quanto pare OpenGl è una state machine globale che risiede nella scheda grafica e non un oggetto instanziabile.
		// Dato che ogni thread può avere un solo contesto OpenGl attivo, la funzione glfwMakeContextCurrent stabilisce
		// che il contesto su cui lavorare è la finestra che sto creando, se non chiamassi questa funzione ogni chiamata
		// ad una funzione OpenGL crasherebbe
		glfwMakeContextCurrent(window);

		//REVIEW - review da fininre
		// GLAD ossia un automatismo che trova le funzioni nella gpu dato che vengono internpretata
		// diversamente tra schede grafiche
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "[structs.hpp->s_Engine]: GLAD initialization failed!";
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		
		// std::cout << glGetString(GL_VERSION) << std::endl;

		//REVIEW - Rendering Zone
		// Definisce l'area in cui andrai a renderizzare qualsiasi costrutto grafico, i primi due parametri
		// x e y definiscono l'angolo in basso a sinistra della zona, i parametri width e heigh immagino
		// siano abbastanza chiari. Qeusto mappa le coords OpenGL che vanno da -1 a +1 in entrambi gli assi.
		// Questo sistema viene chiamato NDC = Normalized Device Coordinates
		glViewport(0, 0, WIDTH, HEIGHT);

		lastTime = getDeltaTime();
	}

	~s_Engine()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	float getDeltaTime()
	{
		float currentTime = glfwGetTime();
		float dt = currentTime - lastTime;

		lastTime = currentTime;

		return (dt);
	}
} t_Engine;

#endif

// glfwInit()              	→ inizializza GLFW (nessuna GPU ancora)
// glfwCreateWindow()      	→ crea finestra + contesto OpenGL
// glfwMakeContextCurrent()	→ attiva il contesto su questo thread
// gladLoadGLLoader()      	→ ora che il contesto esiste, carica tutte le funzioni GL
// glViewport()            	→ ora puoi chiamare funzioni GL moderne