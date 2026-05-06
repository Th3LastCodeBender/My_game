#ifndef STRUCTS_HPP
#	define STRUCTS_HPP

#include "header.hpp"

using namespace glm;

typedef struct s_Engine 
{
	GLFWwindow *window;

	s_Engine ()
	{
		//REVIEW - Funzione di inizializzazione di OpenGL, non penso di dover spiegare molto a parte il fatto
		//che nel caso in cui non vada a buon fine restituisce GLFW_FALSE ossia 0
		if (glfwInit() == GLFW_FALSE)
		{
			std::cerr << "[Struct.cpp]: GLFW initialization failed!";
			exit(EXIT_FAILURE);
		}

		window = glfwCreateWindow(WIDTH, HEIGHT, "my gayme", nullptr, nullptr);
		if (!window)
		{
			std::cerr << "[Struct.cpp]: glfw Window failed!";
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		//REVIEW - OpenGL contexts and state machine init
		//A quanto pare OpenGl è una state machine globale che risiede nella scheda grafica e non un oggetto instanziabile.
		//Dato che ogni thread può avere un solo contesto OpenGl attivo, la funzione glfwMakeContextCurrent stabilisce
		//che il contesto su cui lavorare è la finestra che sto creando, se non chiamassi questa funzione ogni chiamata
		//ad una funzione OpenGL crasherebbe
		glfwMakeContextCurrent(window);
		
		//REVIEW - Rendering Zone
		//Definisce l'area in cui andrai a renderizzare qualsiasi costrutto grafico, i primi due parametri
		//x e y definiscono l'angolo in basso a sinistra della zona, i parametri width e heigh immagino
		//siano abbastanza chiari. Qeusto mappa le coords OpenGL che vanno da -1 a +1 in entrambi gli assi.
		//Questo sistema viene chiamato NDC = Normalized Device Coordinates
		glViewport(0, 0, WIDTH, HEIGHT);		
	}
} t_Engine;

#endif