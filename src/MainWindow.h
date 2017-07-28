#pragma once
#include "wx/wxprec.h"
#include "CLogWindow.h"

class MainWindow : public wxFrame
{
private:
	enum 
	{ 
		ID_QUIT = wxID_HIGHEST + 1,
		ID_DISPLAY_LOG
	};

	CLogWindow *mLogWindow;

	// Menu bar
	wxMenuBar *menubar;
	wxMenu *fileMenu;
	wxMenu *viewMenu;
	wxMenuItem* viewItems;

	void createMenuBar();
	void OnDisplayLogWindowCheckbox(wxCommandEvent& event);

	// Current (main) window events
	void OnQuit(wxCommandEvent& event);
	void onClose(wxCloseEvent& evt);
	void OnResize(wxSizeEvent& event);

public:
	wxPanel* mMainPanel;

	MainWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~MainWindow();
	MainWindow(const MainWindow& tw) = delete;
	MainWindow(MainWindow&& tw) = delete;
	MainWindow& operator=(const MainWindow& tw) = delete;
	MainWindow& operator=(MainWindow&&) = delete;
};

