#include "../object/object_manager/object_manager.h"
#include "../object/Object_behavior.cpp"
#include "../object/Component_behavior.cpp"

obj::Camera* camera;

// Indices for vertices order of a square
GLuint sqrIndices[] = {
    0, 2, 1, // Upper triangle
    0, 3, 2, // Lower triangle
};

struct renderArguments {
public:
    GLFWwindow* window;
};

namespace debug {
	template <class T>
	struct vectorPair {
	public:
		std::vector<T> v1 = std::vector<T>();
		std::vector<T> v2 = std::vector<T>();

		void add(T val1, T val2) {
			v1.push_back(val1);
			v2.push_back(val2);
		}

		void pop_back(T* val1, T* val2) {
			if (v1.size() == 0) return;
			*val1 = v1.back();
			*val2 = v2.back();

			v1.pop_back();
			v2.pop_back();
		}

		
	};

	vectorPair<Vector2> lines = vectorPair<Vector2>();

	GLfloat* getLineVertices() {
		// Six values because it defines two positions in 3D space
		GLfloat* vert = new GLfloat[6];
		Vector2* v1 = nullptr;
		Vector2* v2 = nullptr;
		lines.pop_back(v1, v2);
		if (v1 == nullptr) {
			delete v1, v2;
			delete[] vert;
			return nullptr;	
		}
		vert[0] = v1->x;
		vert[1] = v1->y;
		vert[2] = 0;
		vert[3] = v2->x;
		vert[4] = v2->y;
		vert[5] = 0;

		return vert;
	}
}

Vector2 camOperations(Vector2 point) {
    point -= camera->position;
    point /= camera->getZoom();

    return point;
}

GLfloat* calcObjectVertices(object* obj) {
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

static void render(renderArguments args) {
    // Specify color of background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	// Render objects
    for (unsigned long int i = 0; i < obj_m::objects.size(); i++) {
        if (obj_m::objects[i]->image == NULL) continue;
        GLfloat* vertices = calcObjectVertices(obj_m::objects[i]);

        // Generate Vertex Array Object and bind
        VAO vao;
        vao.Bind();

        // Generate Vertex Buffer Object and link it to vertices
        VBO vbo(vertices, 20 * sizeof(GLfloat));
        // Generate Element Buffer Object and link ti to indices
        EBO ebo(sqrIndices, sizeof(sqrIndices));

        // Link VBO attributes to VAO
        vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, 5 * sizeof(float), (void*)0);
        vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        // Unbind all to prevent accidental modification
        vao.Unbind();
        vao.Unbind();
        ebo.Unbind();

        obj_m::objects[i]->image->Bind();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        vao.Delete();
        vbo.Delete();
        ebo.Delete();
        delete[] vertices;
    }

	// Render lines
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat* lineVertices = debug::getLineVertices();
	while (lineVertices != nullptr) {
		glVertexPointer(3, GL_FLOAT, 0, lineVertices);
		glDrawArrays(GL_LINES, 0, 2);

		lineVertices = debug::getLineVertices();
	}
	delete[] lineVertices;
	glDisableClientState(GL_VERTEX_ARRAY);

	glfwSwapInterval(1);
    glfwSwapBuffers(args.window);
    glfwPollEvents();
}
