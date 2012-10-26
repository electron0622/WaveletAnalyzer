///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __FRAME_H__
#define __FRAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace WaveletAnalyzer
{
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class IMainFrame
	///////////////////////////////////////////////////////////////////////////////
	class IMainFrame : public wxFrame 
	{
		private:
		
		protected:
			wxMenuBar* mMenubar;
			wxMenu* mMenuFile;
			wxMenu* mMenuSettings;
			wxMenu* mMenuHelp;
			wxPanel* mPanelMain;
			wxStaticLine* mStaticline1;
			wxPanel* mPanelSub;
			wxStaticLine* mStaticline2;
			wxPanel* mPanelSettings;
			wxStatusBar* mStatusBar;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnWindowClose( wxCloseEvent& event ) { event.Skip(); }
			virtual void OnMenuOpen( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnMenuDevice( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnMenuClose( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnMenuExit( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnMenuAbout( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			IMainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Wavelet Analyzer 0.0.0 alpha"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
			
			~IMainFrame();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class ISettingFrame
	///////////////////////////////////////////////////////////////////////////////
	class ISettingFrame : public wxFrame 
	{
		private:
		
		protected:
			wxNotebook* mNotebookSettings;
			wxPanel* mPanel1;
			wxPanel* mPanel2;
		
		public:
			
			ISettingFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
			
			~ISettingFrame();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class IAboutDialog
	///////////////////////////////////////////////////////////////////////////////
	class IAboutDialog : public wxDialog 
	{
		private:
		
		protected:
			wxPanel* mPanelAbout;
			wxStaticText* mStaticTextTitle;
			wxStaticText* mStaticTextVersion;
			wxStaticText* mStaticTextSpace1;
			wxStaticText* mStaticTextDescription;
			wxStaticText* mStaticTextSpace2;
			wxStaticText* mStaticTextDevelopedBy;
			wxStaticText* mStaticTextDeveloper1;
			wxStaticText* mStaticTextSpace3;
			wxButton* mButtonOK;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnButtonOK( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			IAboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE ); 
			~IAboutDialog();
		
	};
	
} // namespace WaveletAnalyzer

#endif //__FRAME_H__
