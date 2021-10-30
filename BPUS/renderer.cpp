#include "Object_Manager.h"
#include "Object_behavior.cpp"

Obj::Camera* camera;

// Indices for vertices order of a square
GLuint sqrIndices[] = {
	0, 2, 1, // Upper triangle
	0, 3, 2, // Lower triangle
};

struct RenderArguments {
public:
	GLFWwindow* window;
	Shader* shader;

	GLuint scaleUni;
};

Vector2 camOperations(Vector2 point) {
	point -= camera->position;
	point /= camera->getZoom();

	return point;
}

GLfloat* calcObjectVertices(Object* obj) {
	GLfloat* vertices = new GLfloat[4 * 5];

	Vector2 position = camOperations(obj->position);
	Vector2 size = obj->size / camera->getZoom();
	Vector2 pivot = size / 2;
	// Lower left
	Vector2 pos0 = (Vector2(0, 0) - pivot).rotate(obj->rotation);
	pos0 += position + pivot;
	// Upper left
	Vector2 pos1 = (Vector2(-pivot.x, pivot.y)).rotate(obj->rotation);
	pos1 += position + pivot;
	// Upper right
	Vector2 pos2 = (Vector2(0, 0) + pivot).rotate(obj->rotation);
	pos2 += position + pivot;
	// Lower right
	Vector2 pos3 = (Vector2(pivot.x, -pivot.y)).rotate(obj->rotation);
	pos3 += position + pivot;

	double ratio = (double)camera->getWindowDimensions().y / (double)camera->getWindowDimensions().x;
	pos0.x *= ratio;
	pos1.x *= ratio;
	pos2.x *= ratio;
	pos3.x *= ratio;

	// Lower left corner
	vertices[0] = pos0.x;
	vertices[1] = pos0.y;
	vertices[2] = 0.0f;
	// Texture coordinates
	vertices[3] = 0.0f;
	vertices[4] = 0.0f;
	
	// Upper left corner
	vertices[5] = pos1.x;
	vertices[6] = pos1.y;
	vertices[7] = 0.0f;
	// Texture coordinates
	vertices[8] = 0.0f;
	vertices[9] = 1.0f;
	
	// Upper right corner
	vertices[10] = pos2.x;
	vertices[11] = pos2.y;
	vertices[12] = 0.0f;
	// Texture coordinates
	vertices[13] = 1.0f;
	vertices[14] = 1.0f;

	// Lower right corner
	vertices[15] = pos3.x;
	vertices[16] = pos3.y;
	vertices[17] = 0.0f;
	// Texture coordinates
	vertices[18] = 1.0f;
	vertices[19] = 0.0f;

	return vertices;
}

static void render(RenderArguments args) {
	// Specify color of background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	args.shader->Activate();
	glUniform1f(args.scaleUni, 0.0f);

	for (int i = 0; i < Obj_M::objects.size(); i++) {
		if (Obj_M::objects[i]->image == NULL) continue;
		GLfloat* vertices = calcObjectVertices(Obj_M::objects[i]);

		// Generate Vertex Array Object and bind
		VAO vao;
		vao.Bind();

		// Generate Vertex Buffer Object and link to vertices
		VBO vbo(vertices, 20 * sizeof(GLfloat));
		// Generate Element Buffer Object and link to indices
		EBO ebo(sqrIndices, sizeof(sqrIndices));

		// Link VBO attributes to VAO
		vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, 5 * sizeof(float), (void*)0);
		vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		// Unbind all to prevent accidental modification
		vao.Unbind();
		vao.Unbind();
		ebo.Unbind();

		Obj_M::objects[i]->image->Bind();
		vao.Bind();
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		vao.Delete();
		vbo.Delete();
		ebo.Delete();
		delete[] vertices;
	}

	glfwSwapBuffers(args.window);
	glfwPollEvents();	
}
