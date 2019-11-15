#ifndef _APP
#define _APP 

#include "ALICE_DLL.h"

using namespace Alice;

#include "CONTROLLER.h" // keyboard and mouse tracker
#include "MODEL.h"// picking and selection
#include "utilities.h"

// ----------------------------------------------------------------------------- CUDA INCLUDES & DECLARATIONS

#ifdef _CUDA_

#include "channel_descriptor.h"
#include "cuda_runtime_api.h"
#include "vector_types.h"	
#include "vector_functions.h"
#include <cuda_gl_interop.h>

#endif // _CUDA

float  					gTotalTimeElapsed = 0;
int 					gTotalFrames = 0;
GLuint 					gTimer;

void init_timer()
{
	glGenQueries(1, &gTimer);
}

void start_timing()
{
	glBeginQuery(GL_TIME_ELAPSED, gTimer);
}

float stop_timing()
{
	glEndQuery(GL_TIME_ELAPSED);

	GLint available = GL_FALSE;
	while (available == GL_FALSE)
		glGetQueryObjectiv(gTimer, GL_QUERY_RESULT_AVAILABLE, &available);

	GLint result;
	glGetQueryObjectiv(gTimer, GL_QUERY_RESULT, &result);

	float timeElapsed = result / (1000.0f * 1000.0f * 1000.0f);
	return timeElapsed;
}

void displayFPS(float timeElapsed)
{
	gTotalFrames++;
	gTotalTimeElapsed += timeElapsed;
	float fps = gTotalFrames / gTotalTimeElapsed;
	char string[1024] = { 0 };
	sprintf(string, "FPS: %0.2f FPS", fps);
	glutSetWindowTitle(string);
}

#ifdef _CUDA_

char *ref_file = NULL;

//------------------------------------------------------------------------------- CUDA 

bool cudaInited = false;

static void checkCUDAError(const char *msg)
{
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err) {
		fprintf(stderr, "Cuda error: %s: %s.\n", msg, cudaGetErrorString(err));
		exit(EXIT_FAILURE);
	}
}
inline int _ConvertSMVer2Cores(int major, int minor)
{
	// Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
	typedef struct {
		int SM; // 0xMm (hexidecimal notation), M = SM Major version, and m = SM minor version
		int Cores;
	} sSMtoCores;

	sSMtoCores nGpuArchCoresPerSM[] =
	{
		{ 0x20, 32 }, // Fermi Generation (SM 2.0) GF100 class
		{ 0x21, 48 }, // Fermi Generation (SM 2.1) GF10x class
		{ 0x30, 192}, // Kepler Generation (SM 3.0) GK10x class
		{ 0x32, 192}, // Kepler Generation (SM 3.2) GK10x class
		{ 0x35, 192}, // Kepler Generation (SM 3.5) GK11x class
		{ 0x37, 192}, // Kepler Generation (SM 3.7) GK21x class
		{ 0x50, 128}, // Maxwell Generation (SM 5.0) GM10x class
		{ 0x52, 128}, // Maxwell Generation (SM 5.2) GM20x class
		{ 0x53, 128}, // Maxwell Generation (SM 5.3) GM20x class
		{ 0x60, 64 }, // Pascal Generation (SM 6.0) GP100 class
		{ 0x61, 128}, // Pascal Generation (SM 6.1) GP10x class
		{ 0x62, 128}, // Pascal Generation (SM 6.2) GP10x class
		{ 0x70,  64},
		{ 0x72,  64},
		{ 0x75,  64},
		{   -1, -1 }
	};

	int index = 0;
	while (nGpuArchCoresPerSM[index].SM != -1) {
		if (nGpuArchCoresPerSM[index].SM == ((major << 4) + minor)) {
			return nGpuArchCoresPerSM[index].Cores;
		}
		index++;
	}
	printf("MapSMtoCores undefined SM %d.%d is undefined (please update to the latest SDK)!\n", major, minor);
	return -1;
}
int gpuGetMaxGflopsDeviceId()
{
	int current_device = 0, sm_per_multiproc = 0;
	int max_compute_perf = 0, max_perf_device = 0;
	int device_count = 0, best_SM_arch = 0;
	cudaDeviceProp deviceProp;
	cudaGetDeviceCount(&device_count);

	// Find the best major SM Architecture GPU device
	while (current_device < device_count)
	{
		cudaGetDeviceProperties(&deviceProp, current_device);
		if (deviceProp.major > 0 && deviceProp.major < 9999)
		{
			best_SM_arch = MAX(best_SM_arch, deviceProp.major);
		}
		current_device++;
	}

	// Find the best CUDA capable GPU device
	current_device = 0;
	while (current_device < device_count)
	{
		cudaGetDeviceProperties(&deviceProp, current_device);
		if (deviceProp.major == 9999 && deviceProp.minor == 9999)
		{
			sm_per_multiproc = 1;
		}
		else
		{
			sm_per_multiproc = _ConvertSMVer2Cores(deviceProp.major, deviceProp.minor);
		}

		int compute_perf = deviceProp.multiProcessorCount * sm_per_multiproc * deviceProp.clockRate;

		if (compute_perf > max_compute_perf)
		{
			// If we find GPU with SM major > 2, search only these
			if (best_SM_arch > 2)
			{
				// If our device==dest_SM_arch, choose this, or else pass
				if (deviceProp.major == best_SM_arch)
				{
					max_compute_perf = compute_perf;
					max_perf_device = current_device;
				}
			}
			else
			{
				max_compute_perf = compute_perf;
				max_perf_device = current_device;
			}
		}
		++current_device;
	}
	return max_perf_device;
}
int InitialiseCUDADevice()
{

	printf("\n CUDA INITIALIZED ");

	if (!cudaInited)
	{
		cudaInited = true;
		cudaGLSetGLDevice(gpuGetMaxGflopsDeviceId());
		checkCUDAError(" cudaGLSetGLDevice ");
		printf("\n device set : %i", gpuGetMaxGflopsDeviceId());
	}

	return 1;
}

#endif // _CUDA_

//------------------------------------------------------------------------------- FORWARD DECLARATIONS for functions

void setup();
void update(int value);
void draw();
void keyPress(unsigned char k, int xm, int ym);
void mousePress(int b,int s,int x,int y) ;
void mouseMotion( int x, int y ) ;

//------------------------------------------------------------------------------- APPLICATION VARIABLES


bool HUDSelectOn = false;
bool updateCam = true;
int counter = 0;
string inFile = "";
CONTROLLER CONTROLLERS;
MODEL SCENE;
ButtonGroup B, B1, B2, B3, B4, B5;
SliderGroup S;

//------------------------------------------------------------------------------- CALLBACKS
void updateCallBack( int value )
{
	long start = GetTickCount();

		update( value ) ;
		CONTROLLERS.update(SCENE);

		frame ++ ;
		winW = glutGet(GLUT_WINDOW_WIDTH);
		winH = glutGet(GLUT_WINDOW_HEIGHT);
		
		glutPostRedisplay();					// refresh your screen i.e internally this will call draw()
		glutTimerFunc(10,updateCallBack,0);		// call update every 10 millisecs

	long end = GetTickCount();
	elapsedTime = end - startTime ;
	long time = (end-start) ;
	if( time < 10 )time = 10 ;
}

void drawCallBack()
{
	long start = GetTickCount();
	
	float currentColor[4];
	glGetFloatv(GL_CLEAR, currentColor);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//////////////////////////////////////////////////////////////////////////

	if (updateCam)updateCamera();

	glColor3f(1, 1, 1);

	draw();

	SCENE.draw();
	CONTROLLERS.update(SCENE); // !!!! temp
	CONTROLLERS.draw(SCENE);
	S.draw();
	B.draw();
	B1.draw();
	B2.draw();
	B3.draw();
	B4.draw();
	B5.draw();


	//////////////////////////////////////////////////////////////////////////

	glutSwapBuffers();
	
	long end = GetTickCount();
	elapsedTime = end - startTime ;
	long time = (end-start) ;
	if( time < 10 )time = 10 ;	
}

void keyPressCallBack(unsigned char k, int xm, int ym)
{
	CONTROLLERS.keyPress(k, xm, ym ,SCENE);

	if (k == 'R')setup();
	if( k == 'X' )exit(0) ;
	if( k == 'V' )resetCamera() ;
	if( k == 'T' )topCamera();
	
	keyPress(k,xm,ym);
}

void mousePressCallBack(int b,int s,int x,int y)
{
	 CONTROLLERS.mousePress(b, s, x, y);

	 if (GLUT_LEFT_BUTTON == b && GLUT_DOWN == s)
	 {
		 B.performSelection(x, y);
		 B1.performSelection(x, y);
		 B2.performSelection(x, y);
		 B3.performSelection(x, y);
		 B4.performSelection(x, y);
		 B5.performSelection(x, y);

		 S.performSelection(x, y, HUDSelectOn);
	 }

	 mousePress( b, s, x, y) ;

	 updateCam = (glutGetModifiers() == GLUT_ACTIVE_ALT) ? false : true;
	 
	 if(updateCam )Mouse( b, s, x, y) ;
}

void motionCallBack( int x, int y )
{
	CONTROLLERS.mouseMotion(x, y);

	S.performSelection(x, y, HUDSelectOn);
	B.performSelection(x, y);
	B1.performSelection(x, y);
	B2.performSelection(x, y);
	B3.performSelection(x, y);
	B4.performSelection(x, y);
	B5.performSelection(x, y);

	mouseMotion(x,y);
	if(!HUDSelectOn && updateCam)Motion( x, y) ;
}

//------------------------------------------------------------------------------- ENTRY POINT 
int main(int argc,char** argv)
{
	if (argc > 1)inFile = argv[1];

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(winW,winH);
	glutInitWindowPosition(0,0);
	glutCreateWindow("zSPACE - Alice Viewer");

	// register event methods;
	glutDisplayFunc(drawCallBack);			// register a drawing code 
	glutReshapeFunc(reshape);				// register a reshape 
	glutMouseFunc(mousePressCallBack);
	glutMotionFunc(motionCallBack);
	glutIdleFunc(idle);						//(NEW) calls our idle function
	glutTimerFunc(0,updateCallBack,0);		// call update once in main
	glutKeyboardFunc( keyPressCallBack );	// register keypress function;

	glewInit();

	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else 
		printf("OpenGL 2.0 not supported\n");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);

	setup();
	
	init_timer();
	glutMainLoop();
}

#endif