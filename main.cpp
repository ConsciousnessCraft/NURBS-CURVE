#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <sstream>
#include<string>
#include<tuple>
#include <armadillo>

using namespace std;




void affichage(void);
void clavier(unsigned char touche, int x, int y);
void affiche_repere(void);
/*void afficherCourbe(void);*/


void mouse(int, int, int, int);
void mouseMotion(int, int);
//void reshape(int,int);
float t = .5;

// variables globales pour OpenGL
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance = 0.;

// constantes pour les materieux
float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
float mat_ambient_color[] = { 0.8f, 0.8f, 0.2f, 1.0f };
float mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float no_shininess = 0.0f;
float low_shininess = 5.0f;
float high_shininess = 100.0f;
float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };


void initOpenGl()
{

	//lumiere 

	glClearColor(.5, .5, 0.5, 0.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat l_pos[] = { 3.,3.5,3.0,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_pos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l_pos);
	glEnable(GL_COLOR_MATERIAL);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	//glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	// glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE|GLUT_RGB);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)200 / (GLfloat)200, 0.1f, 10.0f);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0., 0., 4., 0., 0., 0., 0., 1., 0.);

}

int choice = 0;
int rad = -1;

// Control points
arma::mat controlPoints = {
	{-1.0, 0.0, 0},
	{-0.5, 0.5, -0.5},
	{0., 0.5, 0.5},
	{0.5, 0.0, -1},
	{1,0.5,-1.5},
	{1.5,0,0}

};




arma::vec weights = { 1.0, 2.0, 3.0, 1.0,1.0,1.0};
int degree = 2;
//knotVector length should be nbr cntrl points + degree + 1
arma::vec knotVector = { 0,0,1,2,3,4,5,6,6 };

// calculate the basis function  using Cox-de Boor recursion
float calculateBasisFunction(int i, int p, float t) {
	if (p == 0) {
		if (knotVector[i] <= t && t < knotVector[i + 1]) {
			return 1.0;
		}

		else {

			return 0.0;
		}
	}

	float left = 0;
	float right = 0;
	float denominator1 = knotVector[i + p] - knotVector[i];
	float denominator2 = knotVector[i + p + 1] - knotVector[i + 1];
    // the 2 next if will serve the 0/0 problem. if its 0 left remain 0, same for right.
	if (denominator1 != 0) {
		left = (t - knotVector[i]) / denominator1;
	}

	if (denominator2 != 0) {
		right = (knotVector[i + p + 1] - t) / denominator2;
	}

	return left * calculateBasisFunction(i, p - 1, t) + right * calculateBasisFunction(i + 1, p - 1, t);
}

//evaluate the NURBS curve at parameter t
arma::vec evaluateNURBS(float t) {

	arma::vec result(3, arma::fill::zeros);

	for (int i = 0; i < controlPoints.n_rows; ++i) {
		float basis = calculateBasisFunction(i, degree, t);
		for (int j = 0; j < 3; ++j) {
			result(j) += controlPoints(i, j) * weights(i) * basis;

		}
	}


	//dividing by the sum of basis functions times weights
	float sumBasisWeights = 0.0;
	for (int i = 0; i < controlPoints.n_rows; ++i) {
		float basis = calculateBasisFunction(i, degree, t);
		sumBasisWeights += weights(i) * basis;


	}
	// Check division by zero
	if (sumBasisWeights != 0.0) {
		result /= sumBasisWeights;
		//cerr << "good." << endl;

	}
	else {

		//cerr << "Returning zero vector." << endl;
	}


	return result;
}
// the finite difference approximation of the first derivative
arma::vec computeFirstDerivativeFiniteDifference(float t, float epsilon = 1e-1) {
	arma::vec point1 = evaluateNURBS(t - epsilon);
	arma::vec point2 = evaluateNURBS(t + epsilon);
	return (point2 - point1) / (2 * epsilon);
}

// the finite difference approximation of the second derivative
arma::vec computeSecondDerivativeFiniteDifference(float t, float epsilon = 1e-1) {
	arma::vec point1 = computeFirstDerivativeFiniteDifference(t + epsilon);
	arma::vec point2 = computeFirstDerivativeFiniteDifference(t - epsilon);


	return (point1 - point2) / (2 * epsilon);
}

void computeFrenetFrame(float t, arma::vec& tangent, arma::vec& normal, arma::vec& binormal) {

	tangent = computeFirstDerivativeFiniteDifference(t);
	tangent /= arma::norm(tangent);

	normal = computeSecondDerivativeFiniteDifference(t) - arma::dot(computeSecondDerivativeFiniteDifference(t), tangent) * tangent;
	normal = arma::normalise(normal);

	binormal = cross(tangent, normal);
	binormal /= arma::norm(binormal);


}

float computeCurvature(float t) {

	arma::vec C_prime = computeFirstDerivativeFiniteDifference(t);
	arma::vec C_double_prime = computeSecondDerivativeFiniteDifference(t);
	arma::vec crossProduct = arma::cross(C_prime, C_double_prime);
	float crossProductMagnitude = arma::norm(crossProduct);
	float C_primeMagnitude = arma::norm(C_prime);

	//Compute the curvature | C' x C''| / |C' | ^ 3
	float curvature = crossProductMagnitude / pow(C_primeMagnitude, 3);

	return curvature;
}
void drawOsculatingCircles(float t, arma::vec& pointOnCurve, arma::vec& normal, arma::vec& tangent, int rad) {


	float radius = 1 / computeCurvature(t);

	float centerX = pointOnCurve(0) + (radius * normal(0));
	float centerY = pointOnCurve(1) + (radius * normal(1));
	float centerZ = pointOnCurve(2) + (radius * normal(2));

	const int numSegments = 1000;
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numSegments; ++i) {
		float theta = 2.0f * 3.14 * static_cast<float>(i) / numSegments;
		float x = centerX + radius * (cos(theta) * normal(0) - sin(theta) * tangent(0));
		float y = centerY + radius * (cos(theta) * normal(1) - sin(theta) * tangent(1));
		float z = centerZ + radius * (cos(theta) * normal(2) - sin(theta) * tangent(2));
		glVertex3f(x, y, z);
	}
	glEnd();
	if (rad == 0) {
		// Draw curvature radius 
		arma::vec3 curvatureRadiusEndPoint = pointOnCurve + radius * normal;
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the radius
		glVertex3f(pointOnCurve(0), pointOnCurve(1), pointOnCurve(2));
		glVertex3f(curvatureRadiusEndPoint(0), curvatureRadiusEndPoint(1), curvatureRadiusEndPoint(2));
		glEnd();

	}






}
void drawSurfaceAlongCurve() {
	const int numSegments = 100;
	const float circleRadius = 0.1;
	for (float t = knotVector(degree); t <= knotVector(controlPoints.n_rows); t += 0.001) {

		arma::vec pointOnCurve = evaluateNURBS(t);
		arma::vec tangent, normal, binormal;
		computeFrenetFrame(t, tangent, normal, binormal);
		glBegin(GL_LINE_STRIP);


		for (int i = 0; i < numSegments; ++i) {
			float theta = 2.0f * 3.14 * static_cast<float>(i) / numSegments;


			float circleX = circleRadius * (cos(theta) * normal[0] + sin(theta) * binormal[0]);
			float circleY = circleRadius * (cos(theta) * normal[1] + sin(theta) * binormal[1]);
			float circleZ = circleRadius * (cos(theta) * normal[2] + sin(theta) * binormal[2]);


			float curveX = pointOnCurve[0];
			float curveY = pointOnCurve[1];
			float curveZ = pointOnCurve[2];


			glColor3f(0.47, 0.11, 0.77);
			glVertex3f(curveX + circleX, curveY + circleY, curveZ + circleZ);

		}

		glEnd();
	}
}
void drawcurve() {

	glBegin(GL_LINE_STRIP);
	for (float t = knotVector(degree); t <= knotVector(controlPoints.n_rows); t += 0.001) {

		arma::vec point = evaluateNURBS(t);
		glVertex3f(point(0), point(1), point(2));



	}
	glEnd();

}
float tFrenet = knotVector(degree);
void drawfrenet_plus_osculatingcircle(int rad) {


	arma::vec tangent, normal, binormal;
	computeFrenetFrame(tFrenet, tangent, normal, binormal);

	// Draw the Frenet frame at the NURBS point
	arma::vec pointOnCurve = evaluateNURBS(tFrenet);


	glColor3f(1.0, 0.0, 0.0); // Red for tangent
	glBegin(GL_LINES);
	glVertex3f(pointOnCurve(0), pointOnCurve(1), pointOnCurve(2));
	glVertex3f(pointOnCurve(0) + tangent(0), pointOnCurve(1) + tangent(1), pointOnCurve(2) + tangent(2));
	glEnd();

	glColor3f(0.0, 1.0, 0.0); // Green for normal
	glBegin(GL_LINES);
	glVertex3f(pointOnCurve(0), pointOnCurve(1), pointOnCurve(2));
	glVertex3f(pointOnCurve(0) + normal(0), pointOnCurve(1) + normal(1), pointOnCurve(2) + normal(2));
	glEnd();

	glColor3f(0.0, 0.0, 1.0); // Blue for binormal
	glBegin(GL_LINES);
	glVertex3f(pointOnCurve(0), pointOnCurve(1), pointOnCurve(2));
	glVertex3f(pointOnCurve(0) + binormal(0), pointOnCurve(1) + binormal(1), pointOnCurve(2) + binormal(2));
	glEnd();


	drawOsculatingCircles(tFrenet, pointOnCurve, normal, tangent, rad);

}
void afficherCourbe()
{

	if (choice == 0) {
		// curve only
		drawcurve();
		// link control points with lines for view 
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < controlPoints.n_rows; ++i) {
			float x = controlPoints(i, 0);
			float y = controlPoints(i, 1);
			float z = controlPoints(i, 2);
			glVertex3f(x, y, z);
		}
		glEnd();


	}

	if (choice == 1) {
		// curve + frenet + osculating circle  
		drawcurve();
		drawfrenet_plus_osculatingcircle(1);

	}

	if (choice == 2) {
		// curve + frenet + osculating circle + curvature raduis( the yellow line )  
		drawcurve();
		drawfrenet_plus_osculatingcircle(0);

	}

	// curve + surface  
	if (choice == 3) {
		drawcurve();
		drawSurfaceAlongCurve();

	}

	glPointSize(5);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < controlPoints.n_rows; ++i) {
		float x = controlPoints(i, 0);
		float y = controlPoints(i, 1);
		float z = controlPoints(i, 2);
		glVertex3f(x, y, z);
	}
	glEnd();



}






int main(int argc, char** argv)
{




	//generateKnotVector();
/* initialisation de glut et creation
       de la fenetre */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("ifs");
	/* Initialisation d'OpenGL */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(1.0);

	//ifs = new Ifs();
/* enregistrement des fonctions de rappel */
	glutDisplayFunc(affichage);


	glutKeyboardFunc(clavier);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	//-------------------------------


	//-------------------------------
	initOpenGl();
	//-------------------------------

/* Entree dans la boucle principale glut */
	glutMainLoop();

	return 0;
}
//------------------------------------------------------
void affiche_repere(void)
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(0., 0.);
	glVertex2f(1., 0.);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(0., 0.);
	glVertex2f(0., 1.);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 0., 1.);
	glEnd();
}

//-----------------------------------------------------



//------------------------------------------------------
void affichage(void)
{
	glMatrixMode(GL_MODELVIEW);
	/* effacement de l'image avec la couleur de fond */
	  //	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//       glClearDepth(10.0f);                         // 0 is near, >0 is far

	glPushMatrix();
	glTranslatef(0, 0, cameraDistance);
	glRotatef(cameraAngleX, 1., 0., 0.);
	glRotatef(cameraAngleY, 0., 1., 0.);
	affiche_repere();
	afficherCourbe();

	glPopMatrix();

	/* on force l'affichage du resultat */

	glFlush();
	glutSwapBuffers();

}

//------------------------------------------------------


//------------------------------------------------------
void clavier(unsigned char touche, int x, int y)
{

	switch (touche)
	{
	case '+': //
		tFrenet += 0.008;
		if (tFrenet > knotVector(controlPoints.n_rows)) tFrenet = knotVector(controlPoints.n_rows);
		glutPostRedisplay();
		break;
	case '-': //* ajustement du t
		tFrenet -= 0.008;
		if (tFrenet < knotVector(degree)) tFrenet = knotVector(degree);
		glutPostRedisplay();
		break;
	case 'f': //* affichage en mode fil de fer 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 'p': //* affichage du carre plein 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutPostRedisplay();
		break;
	case 's': //* Affichage en mode Points seuls 
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glutPostRedisplay();
		break;

	case 'q': //*la touche 'q' permet de quitter le programme 
		exit(0);

	case 'c':
		choice = 0;
		glutPostRedisplay();
		break;
	case 'v':
		choice = 1;
		glutPostRedisplay();
		break;
	case 'b':
		choice = 2;
		glutPostRedisplay();
		break;
	case 'n':
		choice = 3;
		glutPostRedisplay();

		break;
	}


}
void mouse(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}

	else if (button == GLUT_MIDDLE_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseMiddleDown = true;
		}
		else if (state == GLUT_UP)
			mouseMiddleDown = false;
	}
}


void mouseMotion(int x, int y)
{
	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)
	{
		cameraDistance += (y - mouseY) * 0.2f;
		mouseY = y;
	}

	glutPostRedisplay();
}

