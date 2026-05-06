#include "header.hpp"

t_Engine engine;

int main()
{
	//REVIEW - glfwWindowShouldClose() GLFW setta una flag interna quando l'utente schiaccia la x di chiusura
	while (!glfwWindowShouldClose(engine.window))
	{
		/*REVIEW - Double Buffering con glfwSwapBuffers()
		Utilizzo di double buffering:
			-La GPU disegna su un buffer "nascosto" mentre ne viene mostrato uno "visibile" dal monitor ossia quello già disegnato
			-Chiamando questa funzione i due buffer si scambiano, senza questa chiamata si vedrebbero flickering e artefatti
			 Mentre la scena viene disegnata a schermo.
		Esempio:
		Hai appena renderizzato una scena e vuoi passare al prossimo frame. La GPU disegna pizel per pixel, quindi
		riempie lo schermo poco a poco (che sono ms), ma se non ci fosse differenziazione tra frame già generato e 
		frame in generazione si verificherebbero artefatti come lo screen tearing
			-Frame in costruzione:     [###........] ← GPU sta ancora disegnando
			-Monitor legge in quel momento: vede metà frame disegnato, metà vecchio
			-Risultato visivo: uno "strappo" orizzontale nell'immagine → tearing
		Sembra quindi ovvio che queste funzioni vadano chiamate alla fine del loopdi disegno del frame...*/
		glfwSwapBuffers(engine.window);

		/*REVIEW - Processazione Events (input)
		Processa tutti gli eventi pendenti (tastiera, mouse, ridimensionamento finestra). 
		Senza questa chiamata la finestra si congela e il SO la segna come "not responding".
		Spiego meglio: Quando crei una finestra il SO genera un evento ogni volta che viene dato un input
		Se non chiami questa funzione che di per se processa questi eventi, si accumulano senza essere eseguiti,
		in questa eventualità il sistema operativo inizia a pensare che questa finestra sia bloccata
		(Not Responding) e quindi smette di funzionare.
		Sempre ovvio che questa chiamata vada fatta alla fine della generazione del frame*/
		glfwPollEvents();
	}
	
	return(0);
}