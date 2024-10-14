#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// Biblioteca manual pra implementação dos shaders
#include "../../../common/include/Shader.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 600, HEIGHT = 600;

int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 9 - Bruno Flores", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros das funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	Shader shader("../../../commonfiles/shaders/vertex.vs", "../../../commonfiles/shaders/fragment.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não estão nos buffers
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	

	glUseProgram(shader.ID);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.3f, 0.8f, 0.8f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(3);
		glPointSize(10);

		glBindVertexArray(VAO);

		glUniform4f(colorLoc, 0.4f, 0.4f, 0.4f, 1.0f);
		glDrawArrays(GL_LINE_LOOP, 0, 18);

		glUniform4f(colorLoc, 0.6f, 0.6f, 0.6f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 18, 3);

		glUniform4f(colorLoc, 0.6f, 0.6f, 0.6f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 21, 3);

		glUniform4f(colorLoc, 0.6f, 0.6f, 0.6f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 24, 3);

		glUniform4f(colorLoc, 0.6f, 0.6f, 0.6f, 1.0f);
		glDrawArrays(GL_LINE_STRIP, 27, 3);

		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	GLfloat vertices[] = {
		// Contorno
		0.0, 0.5, 0.0,
		0.5, 0.3, 0.0,
		0.2, 0.3, 0.0,
		0.1, 0.1, 0.0,
		0.4, 0.0, 0.0,
		0.4, -0.1, 0.0,
		0.1, -0.1, 0.0,
		0.3, -0.5, 0.0,
		0.1, -0.5, 0.0,
		0.0, -0.3, 0.0,
		-0.1, -0.5, 0.0,
		-0.3, -0.5, 0.0,
		-0.1, -0.1, 0.0,
		-0.4, -0.1, 0.0,
		-0.4, 0.0, 0.0,
		-0.1, 0.1, 0.0,
		-0.2, 0.3, 0.0,
		-0.5, 0.3, 0.0,

		// Chapéu
		0.0, 0.5, 0.0,
		0.5, 0.3, 0.0,
		-0.5, 0.3, 0.0,

		// Olhos
		0.05, 0.22, 0.0,
		0.1, 0.22, 0.0,
		0.075, 0.25, 0.0,

		-0.05, 0.22, 0.0,
		-0.1, 0.22, 0.0,
		-0.075, 0.25, 0.0,

		// Boca
		-0.025, 0.15, 0.0,
		0.0, 0.165, 0.0,
		0.025, 0.15, 0.0,
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}