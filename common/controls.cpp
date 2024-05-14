// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; /* The "extern" keyword here is to access
						   the variable "window" declared in tutorialXXX.cpp. 
						 This is a hack to keep the tutorials simple. 
						 Please avoid this.*/
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"
#include <iostream>
#include "stdio.h"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix()		{	return ViewMatrix;		}
glm::mat4 getProjectionMatrix()	{	return ProjectionMatrix;}

const double PI = 3.14159265358979f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

double scrollYoff = 0.0;
double gYoffset = 0;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// here your response to cursor scrolling ...
	// yoffset > 0 means you scrolled up, i think
	//printf("%0.1f\n", xoffset);
	scrollYoff+=yoffset;
	gYoffset = yoffset;
}

glm::vec3 X(1,0,0), Y(0,1,0), Z(0,0,1);

// #define  UIMODE_ORIGINAL

#ifdef UIMODE_ORIGINAL		// the original UI
// Initial position : on +Z
glm::vec3 cameraPos = glm::vec3( 0, 0, 5 ); 

// Initial horizontal angle : toward -Z
float horizontalAngle = PI;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	float verLock = 3.14f/6.0f;
	if(verticalAngle>verLock) verticalAngle = verLock;
	if(verticalAngle<-verLock) verticalAngle = -verLock;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - PI/2.0f), 
		0,
		cos(horizontalAngle - PI/2.0f)
		);

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		cameraPos += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		cameraPos -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		cameraPos += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		cameraPos -= right * deltaTime * speed;
	}

	glfwSetScrollCallback(window, scroll_callback);
	//float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
	float FoV = initialFoV - scrollYoff;

	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
		cameraPos,           // Camera is here
		cameraPos+direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}


#else //my modification to UI

// Initial horizontal angle : toward -Z
float horizontalAngle = PI;
// Initial vertical angle : none
float verticalAngle = -PI;
// Initial Field of View
float initialFoV = 45.0f;

// Initial position : on the origin
glm::vec3 cameraPos;
glm::vec3 oC = glm::vec3( 0, 0, 0 ); 
float radius = 20;
float phi = 0.0;
float phiSpeed = 0.002;
void computeMatricesFromInputs_fix(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - PI/2.0f), 
		0,
		cos(horizontalAngle - PI/2.0f)
		);

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS)
		radius+=0.1;
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS)
		radius-=0.1;
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
	}

	cameraPos = oC + radius*direction;

	glfwSetScrollCallback(window, scroll_callback);
	float FoV = initialFoV - scrollYoff;

	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
		cameraPos,	// Camera is here
		oC,			// and looks here : at the same position, plus "direction"
		up          // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void computeMatricesFromInputs(){

	phi = 0.0;	

	gamepadGetInput();

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos); //xpos ypos��ʼֵΪ���������ģ� ��1024/2, 768/2.
	
	//std::cerr<<"\rxpos:"<<xpos<<" ypos:"<<ypos<<"\n"; 

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle -= mouseSpeed * float(1024/2 - xpos );
	verticalAngle   -= mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 directionCord(cos(verticalAngle) * sin(horizontalAngle),	
		sin(verticalAngle),	cos(verticalAngle) * cos(horizontalAngle));
	glm::vec3 direction = X*directionCord.x + Y*directionCord.y + Z*directionCord.z;
	
	// Right vector
	glm::vec3 rightCord(sin(horizontalAngle + PI/2.0f), 0, cos(horizontalAngle + PI/2.0f));
	glm::vec3 right = X*rightCord.x + Y*rightCord.y + Z*rightCord.z;

	// Up vector
	glm::vec3 up = glm::cross( right, direction );
	glm::vec3 upT = up;

	
	if(glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS)
		phi+=phiSpeed;	
	if(glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		phi-=phiSpeed;
	upT = up*float(cos(phi)) + right*float(sin(phi));
	upT = glm::normalize(upT);

	right = glm::cross( direction, upT );

	X = glm::normalize(right); Y = glm::normalize(upT);	Z = glm::normalize(direction);  

	verticalAngle = 0; horizontalAngle = 0;


	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS)
		radius+=0.1;
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS)
		radius-=0.1;

	if(glfwGetKey( window, GLFW_KEY_LEFT_SHIFT )==GLFW_RELEASE) {
		if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS)		oC += X*0.01f;
		if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS)		oC -= X*0.01f;
		if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS)		oC += Y*0.01f;
		if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS)		oC -= Y*0.01f;
	}

	cameraPos = oC + radius*direction;

	glfwSetScrollCallback(window, scroll_callback);
	float FoV = initialFoV - scrollYoff;

	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
		cameraPos,	// Camera is here
		oC,			// and looks here : at the same position, plus "direction"
		upT          // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}


void gamepadGetInput(){
	static bool gameControllerExist = false;
	if(gameControllerExist!=glfwJoystickPresent(GLFW_JOYSTICK_1)){
		gameControllerExist = glfwJoystickPresent(GLFW_JOYSTICK_1);

		if(gameControllerExist){
			if(glfwJoystickIsGamepad(GLFW_JOYSTICK_1))	std::cerr<<"\ngamepad connected";
			else std::cerr<<"\njoystick connected";
		}
		else{
			std::cerr<<"\ngame controlled disconnected";
			return;
		}
	}

	//int jsAxisCount;	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &jsAxisCount);

	GLFWgamepadstate state;
	if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
	{
		if(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])	oC += X*0.01f;
		if(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])	oC -= X*0.01f;
		if(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])	oC += Y*0.01f;
		if(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])		oC -= Y*0.01f;
	
		float speed = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
		if(fabs(speed)>=0.08)
			horizontalAngle += state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]*0.003;
		speed = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
		if(fabs(speed)>=0.08)
			verticalAngle += state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]*0.003;

		radius += state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]*0.003;
		radius -= state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]*0.003;

		if(state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])	phi-=phiSpeed;
		if(state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER])	phi+=phiSpeed;

		//reset camera
		if(state.buttons[GLFW_GAMEPAD_BUTTON_BACK]){
			horizontalAngle = PI;
			verticalAngle = -PI;
			initialFoV = 45.0f;
			oC = glm::vec3( 0, 0, 0 ); 
			radius = 20;
			phi = 0.0;
			X= glm::vec3(1,0,0), Y= glm::vec3(0,1,0), Z=glm::vec3(0,0,1);
		}
	}
}

#endif

void fractalGetInput(GLdouble &xL, GLdouble &yD, GLdouble  &xR, GLdouble &yT, int &winW, int &winH)
{
	if(glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS){
		double DX, DY;
		glfwGetCursorPos(window, &DX, &DY);
		if((DX<450 && DY<250) ) return;  //TWbar UI
	}

	static double lbtnDnPosX, lbtnDnPosY;

	if(glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS ){
		double px, py;
		glfwGetCursorPos(window, &px, &py);
		if(lbtnDnPosX==-1){
			lbtnDnPosX = px;
			lbtnDnPosY = py;
		}
		else{
			double mx,my;
			mx = -(px - lbtnDnPosX)/winW *(xR-xL);
			my = (py - lbtnDnPosY)/winH *(yT-yD);
			xL+=mx; xR+=mx; yD+=my; yT+=my;
			lbtnDnPosX = px;
			lbtnDnPosY = py;
		}
	}
	else{
		lbtnDnPosX = -1;	
		lbtnDnPosY = -1;
	}

	float zoom = 1.0;

	if(glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ){
		gYoffset = 1;
	}
	if(glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS ){
		gYoffset = -1;
	}
	glfwSetScrollCallback(window, scroll_callback);
	if(gYoffset==0) return;

	zoom = 1.0+ gYoffset*0.1f;
	gYoffset = 0;

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	ypos = winH - ypos+1;
	// Reset mouse position for next frame
	//glfwSetCursorPos(window, 1024/2, 768/2);

	GLdouble px = xpos/float(winW) * (xR-xL) + xL; 
	GLdouble py = ypos/float(winH) * (yT-yD) + yD; 
	xL = (xL - px)/zoom + px, xR = (xR - px)/zoom + px;
	yD = (yD - py)/zoom + py, yT = (yT - py)/zoom + py;
}

glm::vec3 *plightPos = NULL;
float *plightPower = NULL;

void key_callback_for_move_light_fix(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(plightPos==NULL) return;
	if(mods == GLFW_MOD_SHIFT){
		if (key == GLFW_KEY_W && action == GLFW_REPEAT ){
			plightPos->y += 0.1;
		}
		if (key == GLFW_KEY_S && action == GLFW_REPEAT ){
			plightPos->y -= 0.1;
		}
		if (key == GLFW_KEY_A && action == GLFW_REPEAT ){
			plightPos->x -= 0.1;
		}
		if (key == GLFW_KEY_D && action == GLFW_REPEAT ){
			plightPos->x += 0.1;
		}
		if (key == GLFW_KEY_Q && action == GLFW_REPEAT ){
			plightPos->z -= 0.1;
		}
		if (key == GLFW_KEY_E && action == GLFW_REPEAT ){
			plightPos->z += 0.1;
		}
	}
}

void key_callback_for_move_light(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS||action == GLFW_REPEAT){
		if(plightPos!=NULL) {
			if(mods == GLFW_MOD_SHIFT){
				float x = dot(*plightPos,X);
				float y = dot(*plightPos,Y);
				float z = dot(*plightPos,Z);
				if (key == GLFW_KEY_W  )
					y += 0.1;
				if (key == GLFW_KEY_S )
					y -= 0.1;
				if (key == GLFW_KEY_A  )
					x += 0.1;
				if (key == GLFW_KEY_D  )
					x -= 0.1;
				if (key == GLFW_KEY_Q  )
					z -= 0.1;
				if (key == GLFW_KEY_E  )
					z += 0.1;
				*plightPos = X*x + Y*y + Z*z;
			}
		}
		if(plightPower!=NULL){
			float p = mods == GLFW_MOD_SHIFT? 1.0f:0.2f;			
			if (key == GLFW_KEY_EQUAL  )
				*plightPower += p;			
			if (key == GLFW_KEY_MINUS )
				*plightPower -= p;
			std::cerr<<"\r lightPower = "<<*plightPower<<"\t\t";
		}	
	}
	
}

void enableMoveLight(glm::vec3 &litpos, float &litPower)
{
	plightPos = &litpos;
	plightPower = &litPower;
	glfwSetKeyCallback(window, key_callback_for_move_light);
}
