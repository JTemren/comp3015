#include"Camera.h"



Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	cameraPos = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Exports the camera matrix to the Vertex Shader
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) {
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));

}

void Camera::Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS) {
		cameraPos += speed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos += speed * -glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos += speed * -cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += speed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
		cameraPos += speed * cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
		cameraPos += speed * -cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE){
		speed = 0.1f;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick) {
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (height / 2)) / height;

		glm::vec3 newOrintation = glm::rotate(cameraFront, glm::radians(-rotX), glm::normalize(glm::cross(cameraFront, cameraUp)));
		if (!((glm::angle(newOrintation, cameraUp) <= glm::radians(5.0f)) or (glm::angle(newOrintation,-cameraUp) <= glm::radians(5.0f)))){
			cameraFront = newOrintation;
		}

		cameraFront = glm::rotate(cameraFront, glm::radians(-rotY), cameraUp);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;

	}
}