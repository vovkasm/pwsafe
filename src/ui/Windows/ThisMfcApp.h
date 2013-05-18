/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/
#pragma once

/// \file ThisMfcApp.h
/// \brief App object of MFC version of Password Safe
//-----------------------------------------------------------------------------

#include "PasswordSafe.h"
#include "stdafx.h"
#include "SystemTray.h"
#include "PWSRecentFileList.h"
#include "PWSFaultHandler.h"

#include "core/Util.h"
#include "core/PWScore.h"

#include "os/run.h"

#include <afxmt.h>
//-----------------------------------------------------------------------------

// Structure for saving information on what language/help files are installed.
struct LANGHELPFILE {
  LCID lcid;                 // LCID for the language
  std::wstring wsLL;         // 2-character language code
  std::wstring wsCC;         // 2-chharacter country code if needed
  std::wstring wsLanguage;   // Name of language for menu item

  /*
    1... .... 0x80 Current
    .1.. .... 0x40 Help file also present
  */
  BYTE xFlags;               // Flags
};

class DboxMain;

class ThisMfcApp : public CWinApp
{
public:
  ThisMfcApp();
  ~ThisMfcApp();

  HACCEL m_ghAccelTable;

  CPWSRecentFileList* GetMRU() { return m_pMRU; }
  void ClearMRU();
  void AddToMRU(const CString &pszFilename);

  DboxMain *m_pMainDlg;
  PWScore m_core;
  CMenu *m_pMainMenu, *m_pMRUMenu;
  BOOL m_mruonfilemenu;
  HINSTANCE m_hInstResDLL;
  std::vector<LANGHELPFILE> m_vlanguagefiles;

  static const UINT m_uiRegMsg;
  static const UINT m_uiWH_SHELL;

  virtual BOOL InitInstance();
  virtual int ExitInstance();
#if !defined(POCKET_PC)
  virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
#endif

  void EnableAccelerator() { m_bUseAccelerator = true; }
  void DisableAccelerator() { m_bUseAccelerator = false; }
  bool IsAcceleratorEnabled() { return m_bUseAccelerator;}

  BOOL SetTooltipText(LPCWSTR ttt) {return m_pTrayIcon->SetTooltipText(ttt);}
  BOOL IsIconVisible() const {return m_pTrayIcon->Visible();}
  void ShowIcon() {m_pTrayIcon->ShowIcon();}
  void HideIcon() {m_pTrayIcon->HideIcon();}

  // 'STATE' also defined in DboxMain.h - ensure identical
  enum STATE {LOCKED, UNLOCKED, CLOSED};
  void SetSystemTrayState(STATE);
  STATE GetSystemTrayState() const {return m_TrayLockedState;}
  int SetClosedTrayIcon(int &icon, bool bSet = true);

  bool WasHotKeyPressed() {return m_HotKeyPressed;}
  void SetHotKeyPressed(bool state) {m_HotKeyPressed = state;}
  int FindMenuItem(CMenu* Menu, UINT MenuID);
  int FindMenuItem(CMenu* Menu, LPCWSTR MenuString);
  void GetApplicationVersionData();
  void GetDLLVersionData(const CString &cs_dll, int &wLangID);
  CString GetFileVersionString() const {return m_csFileVersionString;}
  CString GetCopyrightString() const {return m_csCopyrightString;}
  CString GetHelpFileName() const {return m_csHelpFile;}
  DWORD GetFileVersionMajorMinor() const {return m_dwMajorMinor;}
  DWORD GetFileVersionBuildRevision() const {return m_dwBuildRevision;}
  void SetACCELTableCreated() {m_bACCEL_Table_Created = true;}
  bool NoSysEnvWarnings() const {return m_noSysEnvWarnings;}
  bool PermitTestdump() const {return m_bPermitTestdump;}
  DWORD GetBaseThreadID() {return m_nBaseThreadID;}
  void GetLanguageFiles();
  void SetLanguage();
  CWnd *SetSystemTrayTarget(CWnd *pWnd)
  {return m_pTrayIcon->SetTarget(pWnd);}

  void SetMinidumpUserStreams(const bool bOpen, const bool bRW, UserStream iStream = usAll);

  DECLARE_MESSAGE_MAP()

protected:
  CPWSRecentFileList* m_pMRU;
  bool m_bUseAccelerator;

private:
  bool ParseCommandLine(DboxMain &dbox, bool &allDone);
  void LoadLocalizedStuff();
  void SetupMenu();
  static BOOL CALLBACK searcher(HWND hWnd, LPARAM lParam);

  HANDLE m_hMutexOneInstance;

  HICON m_LockedIcon;
  HICON m_UnLockedIcon;
  HICON m_ClosedIcon;
  CSystemTray *m_pTrayIcon; // DboxMain needs to be constructed first
  STATE m_TrayLockedState;
  bool m_HotKeyPressed, m_bACCEL_Table_Created;
  DWORD m_dwMajorMinor;
  DWORD m_dwBuildRevision;
  CString m_csFileVersionString;
  CString m_csCopyrightString;
  CString m_csHelpFile;
  int m_AppLangID, m_ResLangID;

  // Following set by command line arguments
  bool m_noSysEnvWarnings; // '-q'
  bool m_bPermitTestdump;  // '--testdump'

  // Used to check if called from a thread
  DWORD m_nBaseThreadID;
};
//-----------------------------------------------------------------------------
// Local variables:
// mode: c++
// End: