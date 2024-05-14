#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
void computeMatricesFromInputs_fix(); //old, stupid control..
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void fractalGetInput(GLdouble &xL, GLdouble &yD, GLdouble  &xR, GLdouble &yT, int &winW, int &winH);
void enableMoveLight(glm::vec3 &litpos, float &litPower);
void gamepadGetInput();
#endif