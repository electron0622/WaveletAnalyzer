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
	
	mMenubar = new wxMenuBar( 0 );
	mMenuFile = new wxMenu();
	wxMenuItem* mMenuItemOpen;
	mMenuItemOpen = new wxMenuItem( mMenuFile, wxID_ANY, wxString( wxT("Open...") ) + wxT('\t') + wxT("Ctrl+O"), wxT("Open the audio file"), wxITEM_NORMAL );
	mMenuFile->Append( mMenuItemOpen );
	
	wxMenuItem* mMenuItemDevice;
	mMenuItemDevice = new wxMenuItem( mMenuFile, wxID_ANY, wxString( wxT("Device...") ) + wxT('\t') + wxT("Ctrl+D"), wxT("Open the audio device"), wxITEM_NORMAL );
	mMenuFile->Append( mMenuItemDevice );
	
	wxMenuItem* mMenuItemClose;
	mMenuItemClose = new wxMenuItem( mMenuFile, wxID_ANY, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mMenuItemClose );
	
	mMenuFile->AppendSeparator();
	
	wxMenuItem* mMenuItemExit;
	mMenuItemExit = new wxMenuItem( mMenuFile, wxID_ANY, wxString( wxT("Exit") ) + wxT('\t') + wxT("Alt-F4"), wxT("Quit Wavelet Analyzer"), wxITEM_NORMAL );
	mMenuFile->Append( mMenuItemExit );
	
	mMenubar->Append( mMenuFile, wxT("&File") ); 
	
	mMenuSettings = new wxMenu();
	mMenubar->Append( mMenuSettings, wxT("&Settings") ); 
	
	mMenuHelp = new wxMenu();
	wxMenuItem* mMenuItemAbout;
	mMenuItemAbout = new wxMenuItem( mMenuHelp, wxID_ANY, wxString( wxT("About...") ) + wxT('\t') + wxT("F1"), wxT("Show about dialog"), wxITEM_NORMAL );
	mMenuHelp->Append( mMenuItemAbout );
	
	mMenubar->Append( mMenuHelp, wxT("&Help") ); 
	
	this->SetMenuBar( mMenubar );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	mPanelMain = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer1->Add( mPanelMain, 1, wxEXPAND, 5 );
	
	mStaticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer1->Add( mStaticline1, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	mPanelSub = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 320,240 ), wxTAB_TRAVERSAL );
	bSizer2->Add( mPanelSub, 0, wxEXPAND, 5 );
	
	mStaticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer2->Add( mStaticline2, 0, wxEXPAND, 5 );
	
	mPanelSettings = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer2->Add( mPanelSettings, 1, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	mStatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( mMenuItemOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuOpen ) );
	this->Connect( mMenuItemDevice->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuDevice ) );
	this->Connect( mMenuItemClose->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuClose ) );
	this->Connect( mMenuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuExit ) );
	this->Connect( mMenuItemAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuAbout ) );
}

IMainFrame::~IMainFrame()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuDevice ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuExit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IMainFrame::OnMenuAbout ) );
	
}

ISettingFrame::ISettingFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	mNotebookSettings = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	mPanel1 = new wxPanel( mNotebookSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mNotebookSettings->AddPage( mPanel1, wxT("a page"), false );
	mPanel2 = new wxPanel( mNotebookSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mNotebookSettings->AddPage( mPanel2, wxT("a page"), false );
	
	bSizer1->Add( mNotebookSettings, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

ISettingFrame::~ISettingFrame()
{
}

IAboutDialog::IAboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	mPanelAbout = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	mStaticTextTitle = new wxStaticText( mPanelAbout, wxID_ANY, wxT("Wavelet Analyzer"), wxDefaultPosition, wxDefaultSize, 0 );
	mStaticTextTitle->Wrap( -1 );
	mStaticTextTitle->SetFont( wxFont( 18, 70, 90, 90, false, wxEmptyString ) );
	
	bSizer2->Add( mStaticTextTitle, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mStaticTextVersion = new wxStaticText( mPanelAbout, wxID_ANY, wxT("Version 0.0.0 alpha"), wxDefaultPosition, wxDefaultSize, 0 );
	mStaticTextVersion->Wrap( -1 );
	bSizer2->Add( mStaticTextVersion, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	mStaticTextSpace1 = new wxStaticText( mPanelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mStaticTextSpace1->Wrap( -1 );
	bSizer2->Add( mStaticTextSpace1, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mStaticTextDescription = new wxStaticText( mPanelAbout, wxID_ANY, wxT("An open source wavelet analyser for the audio signal."), wxDefaultPosition, wxDefaultSize, 0 );
	mStaticTextDescription->Wrap( -1 );
	bSizer2->Add( mStaticTextDescription, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mStaticTextSpace2 = new wxStaticText( mPanelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mStaticTextSpace2->Wrap( -1 );
	bSizer2->Add( mStaticTextSpace2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mStaticTextDevelopedBy = new wxStaticText( mPanelAbout, wxID_ANY, wxT("Developed by:"), wxDefaultPosition, wxDefaultSize, 0 );
	mStaticTextDevelopedBy->Wrap( -1 );
	bSizer2->Add( mStaticTextDevelopedBy, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mStaticTextDeveloper1 = new wxStaticText( mPanelAbout, wxID_ANY, wxT("Sato Takaaki"), wxDefaultPosition, wxDefaultSize, 0 );
	mStaticTextDeveloper1->Wrap( -1 );
	bSizer2->Add( mStaticTextDeveloper1, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mStaticTextSpace3 = new wxStaticText( mPanelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mStaticTextSpace3->Wrap( -1 );
	bSizer2->Add( mStaticTextSpace3, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	mPanelAbout->SetSizer( bSizer2 );
	mPanelAbout->Layout();
	bSizer2->Fit( mPanelAbout );
	bSizer1->Add( mPanelAbout, 1, wxALL|wxEXPAND, 5 );
	
	mButtonOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( mButtonOK, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	mButtonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IAboutDialog::OnButtonOK ), NULL, this );
}

IAboutDialog::~IAboutDialog()
{
	// Disconnect Events
	mButtonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IAboutDialog::OnButtonOK ), NULL, this );
	
}
