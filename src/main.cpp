#include "../includes/header.hpp"
// https://docs.gl/

t_Engine engine;

// static std::string *parseShader(const std::string &filepath, std::string type)
// {
// 	std::ifstream		stream(filepath);
// 	std::string			strFuncts[2];
// 	std::string			line;
// 	std::stringstream 	stringStream;
// 	int					attribute;

// 	attribute = -1;
// 	while (getline(stream, line))
// 	{
// 		if (attribute != -1)
// 		{
// 			stringStream << line;
// 			if (line.empty() || line[0] == '\0' || line.find("};") != std::string::npos)
// 			{
// 				strFuncts[attribute] = stringStream.str();
// 				stringStream.clear();
// 				attribute = -1;		
// 			}
// 			continue;
// 		}
// 		if (line.find('#' + type) != std::string::npos)
// 		{
// 			if (line.find('#' + type + "fragment shader") != std::string::npos)
// 				attribute = VERTEX;
// 			else if (line.find('#' + type + "vertex shader") != std::string::npos)
// 				attribute = FRAGMENT;
// 		}
// 	}
// 	if (strFuncts[0].empty() || strFuncts[0][0] == '\0' || strFuncts[1].empty() || strFuncts[1][0] == '\0')
// 		std::cerr	<< "[parseShader]: Something went wrong while parsing the " 
// 					<< type 
// 					<< " shader function!" 
// 					<< std::endl;
// 	return (strFuncts);
// }

static unsigned int compileShader(unsigned int type, const std::string &source)
{
	unsigned int	id;
	const char*		src;
	int				result;

	id = glCreateShader(type);
	src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *msg = (char *)malloc(length * sizeof(char));
		if(!msg)
			std::cerr << "[compileShader]: Malloc failed!" << std::endl;
		glGetShaderInfoLog(id, length, &length, msg);
		std::cout << "[compileShader]: glCompileShader failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader with id: " << id << std::endl;
		std::cout << msg << std::endl;
		free(msg);
		glDeleteShader(id);
		return (0);
	}
	return id;
}
/**
 * @brief Create a Shader object
 * 
 * @param vertexShader stringified source code of the vertex shader creation function
 * @param fragmentShader stringified source code of the fragment shader creation function
 * @return int 
 */
static unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vShader = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glValidateProgram(program);

	//dopo che sono state caricate come programma nella gpu possiamo cancellarle
	glDetachShader(program, vShader);
	glDetachShader(program, fShader);
	// glDeleteShader(vShader);
	// glDeleteShader(fShader);

	return (program);
}	
int main()
{
	unsigned int	buffer;
	unsigned int 	vaoTriangle;
	float			positions[6] = {
						-0.5f, -0.5f,
						 0.0f,  0.5f,
						-0.5f,  0.5f
					};

	glGenVertexArrays(1, &vaoTriangle);
	glBindVertexArray(vaoTriangle);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (6 * sizeof(float)), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int shader = createShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	//REVIEW - glfwWindowShouldClose() GLFW setta una flag interna quando l'utente schiaccia la x di chiusura
	while (!glfwWindowShouldClose(engine.window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3); 
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
	
	glDeleteProgram(shader);
	glDeleteBuffers(1, &buffer);
	return(0);
}