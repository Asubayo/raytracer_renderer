﻿#include "CGLCanvas.h"

BEGIN_EVENT_TABLE(CGLCanvas, wxGLCanvas)
EVT_ENTER_WINDOW(CGLCanvas::onMouseEnter)
END_EVENT_TABLE()

CGLCanvas::CGLCanvas(wxWindow* parent, wxWindowID id,
	const int* attribList, const wxPoint& pos, const wxSize& size,
	long style, const wxString& name, const wxPalette& palette)
	: wxGLCanvas(parent, id, attribList, pos, size, style, name, palette)
{
	Bind(wxEVT_PAINT, &CGLCanvas::onPaint, this);
	Bind(wxEVT_TIMER, &CGLCanvas::onTimer, this);

	m_context = std::make_unique<wxGLContext>(this);
	SetCurrent(*m_context);

	initializeGLEW();

	setupControls();

	// Initialize timer
	mTimer = std::make_unique<wxTimer>(this, TIMERNUMBER);
	mTimer->Start(INTERVAL);
	mStartTime = std::chrono::high_resolution_clock::now();

	gldriver = new CGLDriver();
	mRenderer = new CubeTest(gldriver, NULL);
	mRenderer->init(gldriver, NULL);
}

CGLCanvas::~CGLCanvas()
{
	if (mRenderer != NULL)
	{
		delete mRenderer;
		mRenderer = nullptr;
	}

	if (gldriver != NULL)
	{
		delete gldriver;
		gldriver = nullptr;
	}

	SetCurrent(*m_context);

	mTimer->Stop();

	Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CGLCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(CGLCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_MOTION, wxMouseEventHandler(CGLCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CGLCanvas::onMouseWheel), NULL, this);
	Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(CGLCanvas::onKeyDown), NULL, this);
}

/*void 
CGLCanvas::setRenderer(CRenderer *renderer)
{
	mRenderer = renderer;
}*/

void 
CGLCanvas::resize()
{
	glViewport(0, 0, GetSize().GetWidth(), GetSize().GetHeight());
	Refresh();
}

void 
CGLCanvas::initializeGLEW()
{
	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK) 
	{
		const GLubyte* msg = glewGetErrorString(err);
		throw std::exception(reinterpret_cast<const char*>(msg));
	}
}

void 
CGLCanvas::onTimer(wxTimerEvent& event)
{
	ProcessEvent(wxPaintEvent());
}

void 
CGLCanvas::setupControls()
{
	mLastMouseX = 300;
	mLastMouseY = 300;
	mFirstMouse = true;

	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CGLCanvas::onMouseEvent), NULL, this);
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CGLCanvas::onMouseEvent), NULL, this);
	Connect(wxEVT_MOTION, wxMouseEventHandler(CGLCanvas::onMouseEvent), NULL, this);
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CGLCanvas::onMouseWheel), NULL, this);
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CGLCanvas::onKeyDown), NULL, this);
}

void 
CGLCanvas::onPaint(wxPaintEvent& event)
{
	SetCurrent(*m_context);
	
	auto t_now = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(t_now - mStartTime).count();

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//auto m_llTimerStartTick = wxGetLocalTimeMillis();
	// ...
	//llTimerStopTick = wxGetLocalTimeMillis();
	//const wxLongLong llElapsedTime = llTimerStopTick - m_llTimerStartTick;

	mRenderer->render();

	glFlush();
	SwapBuffers();
}

void 
CGLCanvas::onMouseEnter(wxMouseEvent& WXUNUSED(ev))
{
	SetFocus();
}

void 
CGLCanvas::onKeyDown(wxKeyEvent& event)
{
	int code = event.GetKeyCode();
	auto cam = mRenderer->getCam();

	if (cam)
	{
		if (code == 'z' || code == 'Z')
		{
			cam->processKeyboard(FORWARD, 0.005);
		}

		if (code == 's' || code == 'S')
		{
			cam->processKeyboard(BACKWARD, 0.005);
		}

		if (code == 'd' || code == 'D')
		{
			cam->processKeyboard(RIGHT, 0.005);
		}

		if (code == 'q' || code == 'Q')
		{
			cam->processKeyboard(LEFT, 0.005);
		}
	}
}

void 
CGLCanvas::onMouseWheel(wxMouseEvent& event)
{
	float value = 0;
	if(event.GetWheelRotation() > 0) 
	{
		value += (event.GetWheelDelta() / 240.0);
	}
	else if(event.GetWheelRotation() < 0) 
	{
		value -= (event.GetWheelDelta() / 240.0);
	}

	auto cam = mRenderer->getCam();
	if (cam)
	{
		cam->processMouseWheel(value);
	}
}

void 
CGLCanvas::onMouseEvent(wxMouseEvent& event)
{
	if (event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_LEFT)) 
	{
		if (mFirstMouse)
		{
			mLastMouseX = event.GetX();
			mLastMouseY = event.GetY();
			mFirstMouse = false;
		}

		GLfloat xoffset = (float)(event.GetX() - mLastMouseX) ;
		GLfloat	yoffset = (float)(mLastMouseY - event.GetY());

		mLastMouseX = event.GetX();
		mLastMouseY = event.GetY();
		auto cam = mRenderer->getCam();

		if (cam)
		{
			cam->processMouseMovement(xoffset, yoffset);
		}
	}
	else
	{
		mFirstMouse = true;
	}
}