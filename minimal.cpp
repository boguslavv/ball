/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWidgets sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include <vector>
#include <iostream>
#include <utility>
#include <random>

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include <wx/timer.h>
#include <wx/dcbuffer.h>
#include <wx/listctrl.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "sample.xpm"
#endif

struct sBall
{
	float px, py;
	float vx, vy;
	float ax, ay;
	float radius;
	float mass;

	int id;
};

wxList ListOfCanvases;                 //Lista aktualnych "Canvasow"

class MyFrame;

//class MyCanvas: public wxScrolledWindow
class MyCanvas: public wxWindow
{
 public:
    MyCanvas( MyFrame *parent );
    //MyCanvas1(wxFrame *frame, int x, int y, int w, int h);
    void OnPaint(wxPaintEvent &event);
    void Draw(wxDC& pdc);

    void DrawCircles(wxDC& dc);

    std::vector<sBall> vecBalls;

    // Adds a ball to the vector
	void AddBall(float x, float y, float r = 5.0f)
	{
		sBall b;
		b.px = x*15; b.py = y*15;
		b.vx = 1; b.vy = 1;
		b.ax = 5; b.ay = 5;
		b.radius = r;
		b.mass = r * 10.0f;

		b.id = vecBalls.size();
		vecBalls.emplace_back(b);
        std::cout << b.px << ", " << b.py << std::endl;
	}

    wxDECLARE_EVENT_TABLE();
};

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit() wxOVERRIDE;
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);
    void OnTimer(wxTimerEvent& event);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void SimStart(wxCommandEvent& event);
    void SimStop(wxCommandEvent& event);

    MyCanvas   *m_canvas;


private:
    wxTimer m_timer;
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = wxID_EXIT,
    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT,
    Minimal_Simulation,
    Sim_Start,
    Sim_Stop,
    TIMER_ID
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Sim_Start, MyFrame::SimStart)
    EVT_MENU(Sim_Stop, MyFrame::SimStop)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
    EVT_TIMER(TIMER_ID, MyFrame::OnTimer)
wxEND_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
wxIMPLEMENT_APP(MyApp);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

    // create the main application window
    MyFrame *frame = new MyFrame("Minimal wxWidgets App");

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}


// ----------------------------------------------------------------------------
// MyCanvas
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
wxBEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT  (MyCanvas::OnPaint)
wxEND_EVENT_TABLE()


// MyCanvas::MyCanvas(MyFrame *parent)
//         : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
//                            wxHSCROLL | wxVSCROLL)
// {

// };

MyCanvas::MyCanvas(MyFrame *parent)
        : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{

    //SetBackgroundColour(*wxCYAN);
    SetBackgroundColour(*wxBLUE);
    //SetBackgroundColour(wxColour(255, 0, 0));

    const wxSize size = GetVirtualSize();
    // Add 10 Random Balls
    for (int i = 0; i <5; i++){
        // auto const seed = 123;
        // auto urbg = std::mt19937 {seed};
        // double const mu = 5.0;
        // double const sigma = 2.0;
        // auto norm = std::normal_distribution<double>{mu,sigma};
        // int value = int(std::round(norm(urbg)));
        // AddBall(value % size.x, value % size.y, rand() % 16 + 2);
        AddBall(rand() % size.x, rand() % size.y, rand() % 16 + 2);
    }
    //https://hackingcpp.com/cpp/std/random.html


};

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
        wxPaintDC pdc(this);
        Draw(pdc);
}

void MyCanvas::Draw(wxDC& pdc)
{
    wxDC &dc = pdc;

    // Adjust scrolled contents for screen drawing operations only.
    // if ( wxDynamicCast(&pdc, wxBufferedPaintDC) ||
    //      wxDynamicCast(&pdc, wxPaintDC) )
    // {
    //     PrepareDC(dc);
    // }

    dc.Clear();

    PrepareDC( dc );

    auto OnScreen = [&]() {

        return nullptr;
    };

    auto UpdateBallPositions = [&]() {
        for (auto &ball : vecBalls)
		{
			// // // Add Drag to emulate rolling friction
			// ball.ax = ball.vx * 1.0f;
			// ball.ay = ball.vy * 1.0f;

			// // // Update ball physics
			// ball.vx += ball.ax*1;
			// ball.vy += ball.ay*1;

            ball.px += ball.ax;
            ball.py += ball.ay;

            const wxSize size = GetVirtualSize();
            if (ball.px > size.x-ball.radius) { ball.ax = -2.5; }
            if (ball.px < ball.radius) { ball.ax = 2.5; }
            if (ball.py > size.y-ball.radius) { ball.ay = -2.5; }
            if (ball.py < ball.radius) { ball.ay = 2.5; }
        }
    };

    UpdateBallPositions();
    DrawCircles(dc);

}

void MyCanvas::DrawCircles(wxDC& dc)
{
    static int x = 100,
        y = 100,
        r = 20;
    // static int directx = 5;
    // static int directy = 5;

    //dc.SetBackground(wxBrush(wxColour(255, 0, 0)));

    dc.SetPen( *wxRED_PEN );
    dc.SetBrush( *wxGREEN_BRUSH );

    const wxSize size = GetVirtualSize();
    for ( int x = 0; x < size.x; x += 15 )
    {
        dc.DrawLine(x, 0, x, size.y);
    }

    for ( int y = 0; y < size.y; y += 15 )
    {
        dc.DrawLine(0, y, size.x, y);
    }


    dc.DrawText("Some circles", 10, 10);

    dc.DrawCircle(x, y, r);
    // dc.DrawCircle(x + 2*r + 5, y, r);
    // dc.DrawCircle(x + 4*r + 5, y, r);

    // if (x > size.x-20) {
    //     directx = -5;
    // }
    // if (x < 20) {
    //     directx = 5;
    // }

    // if (y > size.y-20) {
    //     directy = -5;
    // }
    // if (y < 20) {
    //     directy = 5;
    // }


    // x = x + directx;
    // y = y + directy;

    // Draw Balls
	for (auto ball : vecBalls)
    {
        dc.DrawCircle(ball.px, ball.py, ball.radius);
    }
}



// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title), m_timer(this, TIMER_ID)
{
    // set the frame icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    wxMenu *simMenu = new wxMenu;
    simMenu->Append(Sim_Start, "&Star", "Simulation start");
    simMenu->Append(Sim_Stop, "&Stop", "Simulation stop");

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");


    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(simMenu, "&Simulation");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
// #else // !wxUSE_MENUS
//     // If menus are not available add a button to access the about box
//     wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
//     wxButton* aboutBtn = new wxButton(this, wxID_ANY, "About...");
//     aboutBtn->Bind(wxEVT_BUTTON, &MyFrame::OnAbout, this);
//     sizer->Add(aboutBtn, wxSizerFlags().Center());
//     SetSizer(sizer);
#endif // wxUSE_MENUS/!wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");
#endif // wxUSE_STATUSBAR

    m_canvas = new MyCanvas( this );
    //m_canvas->SetScrollbars( 10, 10, 100, 240 );

}


// event handlers

void MyFrame::OnTimer(wxTimerEvent& event)
{
    // do whatever you want to do every second here

    m_canvas->Refresh();

    //std::cout << "*";
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
                 (
                    "Welcome to %s!\n"
                    "\n"
                    "This is the minimal wxWidgets sample\n"
                    "running under %s.",
                    wxVERSION_STRING,
                    wxGetOsDescription()
                 ),
                 "About wxWidgets minimal sample",
                 wxOK | wxICON_INFORMATION,
                 this);
}


void MyFrame::SimStart(wxCommandEvent& WXUNUSED(event))
{
    std::cout << "* Simulation Start" << std::endl;
    m_timer.Start(25);
}

void MyFrame::SimStop(wxCommandEvent& WXUNUSED(event))
{
    std::cout << "* Simulation Stop" << std::endl;
    m_timer.Stop();
}
