#pragma once

#ifndef __AFXWIN_H__
#  error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "MainFrame.h"


class CDirectSoundDemoApp : public CWinApp
{
public:
    CDirectSoundDemoApp(void);
    ~CDirectSoundDemoApp(void);

    // 重写
public:
    virtual BOOL InitInstance();
    // 实现
    DECLARE_MESSAGE_MAP()
private:
};

extern CDirectSoundDemoApp theApp;