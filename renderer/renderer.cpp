#include "../object/object_manager/object_manager.h"
#include "../object/Object_behavior.cpp"
#include "../object/Component_behavior.cpp"

obj::Camera* camera;

// Indices for vertices order of a square
GLuint sqrIndices[] = {
    0, 2, 1, // Upper triangle
    0, 3, 2, // Lower triangle
};

GLuint lineIndices[] = {
	0, 2, 1,
};

struct renderArguments {
public:
    GLFWwindow* window;

	shader* shaderProgram;
};

Vector2 camOperations(Vector2 point) {
    point -= camera->position;
    point /= camera->getZoom();

    return point;
}

namespace debug {
	template <class T>
	struct vectorPair {
	public:
		std::vector<T> v1 = std::vector<T>();
		std::vector<T> v2 = std::vector<T>();
		std::vector<Vector3> color = std::vector<Vector3>();

		void add(T val1, T val2, Vector3 col) {
			v1.push_back(val1);
			v2.push_back(val2);
			color.push_back(col);
			if (col.x > 1 || col.y > 1 || col.z > 1) {
				std::cout << "WARNING: debug::draw_line: color value is above 1" << std::endl;
			}
		}

		bool pop_back(T* val1, T* val2, Vector3* col) {
			if (v1.size() == 0) return false;
			auto test = v1.back();
			*val1 = v1.back();
			*val2 = v2.back();
			*col = color.back();

			v1.pop_back();
			v2.pop_back();
			color.pop_back();

			return true;
		}

		
	};

	vectorPair<Vector2> lines = vectorPair<Vector2>();

	GLfloat* getLineVertices(renderArguments args) {
		// 20 values because it defines 4 positions plus UV coordinates in 3D space
		GLfloat* vert = new GLfloat[20];
		Vector2* v1 = new Vector2();
		Vector2* v2 = new Vector2();
		Vector3* color = new Vector3();
		if (!lines.pop_back(v1, v2, color)) {
			delete v1, v2, color;
			delete[] vert;
			return nullptr;	
		}

		// Set color
		GLint coloruni_location = glGetUniformLocation(args.shaderProgram->ID, "debug_color");
		glUniform4f(coloruni_location, color->x, color->y, color->z, 1.0);

		Vector2 pos1 = camOperations(*v1);
		Vector2 pos2 = camOperations(*v2);
		float width = 0.003;

		double ratio = (double)camera->getWindowDimensions().y / (double)camera->getWindowDimensions().x;
		pos1.x *= ratio;
		pos2.x *= ratio;

		Vector2 diff = (pos2 - pos1).normalized();
		// Dot product to the y-axis is equivalent to the sin of the angle
		float offset_x = diff.dot(Vector2(0, 1)) * width * -ratio;
		// Dot product to the x-axis is equivalent to the cos of the angle
		float offset_y = diff.dot(Vector2(1, 0)) * width;

		vert[0] = pos1.x - offset_x;
		vert[1] = pos1.y - offset_y;
		vert[2] = 0;
		vert[3] = 0;
		vert[4] = 0;

		vert[5] = pos2.x - offset_x;
		vert[6] = pos2.y - offset_y;
		vert[7] = 0;
		vert[8] = 0;
		vert[9] = 1;

		vert[10] = pos2.x + offset_x;
		vert[11] = pos2.y + offset_y;
		vert[12] = 0;
		vert[13] = 1;
		vert[14] = 1;

		vert[15] = pos1.x + offset_x;
		vert[16] = pos1.y + offset_y;
		vert[17] = 0;
		vert[18] = 1;
		vert[19] = 0;

		delete v1, v2, color;
		return vert;
	}

	void draw_line(Vector2 pos1, Vector2 pos2) {
		// Make it a nice, vibrant red as default
		lines.add(pos1, pos2, Vector3(1, 0, 0));
	}
	void draw_line(Vector2 pos1, Vector2 pos2, Vector3 col) {
		lines.add(pos1, pos2, col);
	}
	void draw_ray(Vector2 pos, Vector2 dir) {
		// Same here, red color as default
		lines.add(pos, pos + dir, Vector3(1, 0, 0));
	}
	void draw_ray(Vector2 pos, Vector2 dir, Vector3 col) {
		lines.add(pos, pos + dir, col);
	}
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
	GLfloat* lineVertices = debug::getLineVertices(args);
	GLint col = glGetUniformLocation(args.shaderProgram->ID, "usetex");
	glUniform1i(col, 0);
	while (lineVertices != nullptr) {
		VAO vao;
		vao.Bind();

		VBO vbo(lineVertices, 20 * sizeof(GLfloat));
		EBO ebo(sqrIndices, sizeof(sqrIndices));

		vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, 5 * sizeof(float), (void*)0);
		vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float)));

		vao.Unbind();
		vbo.Unbind();
		ebo.Unbind();

		obj_m::objects[0]->image->Bind();
		vao.Bind();
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		vao.Delete();
		vbo.Delete();
		ebo.Delete();

		delete[] lineVertices;
		lineVertices = debug::getLineVertices(args);
	}
	glUniform1i(col, 1);
	delete[] lineVertices;

	glfwSwapInterval(1);
    glfwSwapBuffers(args.window);
    glfwPollEvents();
}
