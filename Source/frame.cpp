///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "frame.h"

///////////////////////////////////////////////////////////////////////////
using namespace WaveletAnalyzer;

IMainFrame::IMainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_Menubar = new wxMenuBar( 0 );
	m_MenuFile = new wxMenu();
	wxMenuItem* m_MenuItemOpen;
	m_MenuItemOpen = new wxMenuItem( m_MenuFile, wxID_ANY, wxString( wxT("Open...") ) + wxT('\t') + wxT("Ctrl+O"), wxT("Open the audio file"), wxITEM_NORMAL );
	m_MenuFile->Append( m_MenuItemOpen );
	
	wxMenuItem* m_MenuItemDevice;
	m_MenuItemDevice = new wxMenuItem( m_MenuFile, wxID_ANY, wxString( wxT("Device...") ) + wxT('\t') + wxT("Ctrl+D"), wxT("Open the audio device"), wxITEM_NORMAL );
	m_MenuFile->Append( m_MenuItemDevice );
	
	wxMenuItem* m_MenuItemClose;
	m_MenuItemClose = new wxMenuItem( m_MenuFile, wxID_ANY, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_MenuItemClose );
	
	m_MenuFile->AppendSeparator();
	
	wxMenuItem* m_MenuItemExit;
	m_MenuItemExit = new wxMenuItem( m_MenuFile, wxID_ANY, wxString( wxT("Exit") ) + wxT('\t') + wxT("Alt-F4"), wxT("Quit Wavelet Analyzer"), wxITEM_NORMAL );
	m_MenuFile->Append( m_MenuItemExit );
	
	m_Menubar->Append( m_MenuFile, wxT("&File") ); 
	
	m_MenuSettings = new wxMenu();
	m_Menubar->Append( m_MenuSettings, wxT("&Settings") ); 
	
	m_MenuHelp = new wxMenu();
	wxMenuItem* m_MenuItemAbout;
	m_MenuItemAbout = new wxMenuItem( m_MenuHelp, wxID_ANY, wxString( wxT("About...") ) + wxT('\t') + wxT("F1"), wxT("Show about dialog"), wxITEM_NORMAL );
	m_MenuHelp->Append( m_MenuItemAbout );
	
	m_Menubar->Append( m_MenuHelp, wxT("&Help") ); 
	
	this->SetMenuBar( m_Menubar );
	
	wxBoxSizer* m_BoxSizer1;
	m_BoxSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_PanelMain = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_BoxSizer1->Add( m_PanelMain, 1, wxEXPAND, 5 );
	
	m_Staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	m_BoxSizer1->Add( m_Staticline1, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_BoxSizer2;
	m_BoxSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_PanelSub = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 320,240 ), wxTAB_TRAVERSAL );
	m_BoxSizer2->Add( m_PanelSub, 0, wxEXPAND, 5 );
	
	m_Staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_BoxSizer2->Add( m_Staticline2, 0, wxEXPAND, 5 );
	
	m_PanelSettings = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_BoxSizer3;
	m_BoxSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_BoxSizer4;
	m_BoxSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_ButtonPlay = new wxButton( m_PanelSettings, wxID_ANY, wxT("Play"), wxDefaultPosition, wxDefaultSize, 0 );
	m_BoxSizer4->Add( m_ButtonPlay, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ButtonStop = new wxButton( m_PanelSettings, wxID_ANY, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	m_BoxSizer4->Add( m_ButtonStop, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_BoxSizer3->Add( m_BoxSizer4, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_BoxSizer5;
	m_BoxSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_StaticTextSigma = new wxStaticText( m_PanelSettings, wxID_ANY, wxT("sigma:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StaticTextSigma->Wrap( -1 );
	m_BoxSizer5->Add( m_StaticTextSigma, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl1 = new wxTextCtrl( m_PanelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_BoxSizer5->Add( m_textCtrl1, 1, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinBtn1 = new wxSpinButton( m_PanelSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS );
	m_BoxSizer5->Add( m_spinBtn1, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	m_BoxSizer3->Add( m_BoxSizer5, 0, wxEXPAND, 5 );
	
	
	m_PanelSettings->SetSizer( m_BoxSizer3 );
	m_PanelSettings->Layout();
	m_BoxSizer3->Fit( m_PanelSettings );
	m_BoxSizer2->Add( m_PanelSettings, 1, wxEXPAND, 5 );
	
	
	m_BoxSizer1->Add( m_BoxSizer2, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_BoxSizer1 );
	this->Layout();
	m_StatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( IMainFrame::OnWindowClose ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( IMainFrame::OnWindowIdle ) );
	this->Connect( m_MenuItemOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuOpen ) );
	this->Connect( m_MenuItemDevice->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuDevice ) );
	this->Connect( m_MenuItemClose->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuClose ) );
	this->Connect( m_MenuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuExit ) );
	this->Connect( m_MenuItemAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuAbout ) );
	m_ButtonPlay->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IMainFrame::OnPlayButtonClick ), NULL, this );
	m_ButtonStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IMainFrame::OnStopButtonClick ), NULL, this );
}

IMainFrame::~IMainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( IMainFrame::OnWindowClose ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( IMainFrame::OnWindowIdle ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuDevice ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuExit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuAbout ) );
	m_ButtonPlay->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IMainFrame::OnPlayButtonClick ), NULL, this );
	m_ButtonStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IMainFrame::OnStopButtonClick ), NULL, this );
	
}

ISettingFrame::ISettingFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_BoxSizer1;
	m_BoxSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_NotebookSettings = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_Panel1 = new wxPanel( m_NotebookSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_NotebookSettings->AddPage( m_Panel1, wxT("a page"), false );
	m_Panel2 = new wxPanel( m_NotebookSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_NotebookSettings->AddPage( m_Panel2, wxT("a page"), false );
	
	m_BoxSizer1->Add( m_NotebookSettings, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_BoxSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

ISettingFrame::~ISettingFrame()
{
}

IAboutDialog::IAboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_BoxSizer1;
	m_BoxSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_PanelAbout = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* m_BoxSizer2;
	m_BoxSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_StaticTextTitle = new wxStaticText( m_PanelAbout, wxID_ANY, wxT("Wavelet Analyzer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StaticTextTitle->Wrap( -1 );
	m_StaticTextTitle->SetFont( wxFont( 18, 70, 90, 90, false, wxEmptyString ) );
	
	m_BoxSizer2->Add( m_StaticTextTitle, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_StaticTextVersion = new wxStaticText( m_PanelAbout, wxID_ANY, wxT("Version 0.0.0 alpha"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StaticTextVersion->Wrap( -1 );
	m_BoxSizer2->Add( m_StaticTextVersion, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_StaticTextSpace1 = new wxStaticText( m_PanelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_StaticTextSpace1->Wrap( -1 );
	m_BoxSizer2->Add( m_StaticTextSpace1, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_StaticTextDescription = new wxStaticText( m_PanelAbout, wxID_ANY, wxT("An open source wavelet analyser for the audio signal."), wxDefaultPosition, wxDefaultSize, 0 );
	m_StaticTextDescription->Wrap( -1 );
	m_BoxSizer2->Add( m_StaticTextDescription, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_StaticTextSpace2 = new wxStaticText( m_PanelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_StaticTextSpace2->Wrap( -1 );
	m_BoxSizer2->Add( m_StaticTextSpace2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_StaticTextDevelopedBy = new wxStaticText( m_PanelAbout, wxID_ANY, wxT("Developed by:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StaticTextDevelopedBy->Wrap( -1 );
	m_BoxSizer2->Add( m_StaticTextDevelopedBy, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_StaticTextDeveloper1 = new wxStaticText( m_PanelAbout, wxID_ANY, wxT("Sato Takaaki"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StaticTextDeveloper1->Wrap( -1 );
	m_BoxSizer2->Add( m_StaticTextDeveloper1, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_StaticTextSpace3 = new wxStaticText( m_PanelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_StaticTextSpace3->Wrap( -1 );
	m_BoxSizer2->Add( m_StaticTextSpace3, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	m_PanelAbout->SetSizer( m_BoxSizer2 );
	m_PanelAbout->Layout();
	m_BoxSizer2->Fit( m_PanelAbout );
	m_BoxSizer1->Add( m_PanelAbout, 1, wxALL|wxEXPAND, 5 );
	
	m_ButtonOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_BoxSizer1->Add( m_ButtonOK, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	this->SetSizer( m_BoxSizer1 );
	this->Layout();
	m_BoxSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_ButtonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IAboutDialog::OnButtonOK ), NULL, this );
}

IAboutDialog::~IAboutDialog()
{
	// Disconnect Events
	m_ButtonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IAboutDialog::OnButtonOK ), NULL, this );
	
}
