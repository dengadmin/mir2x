/*
 * =====================================================================================
 *
 *       Filename: previewwindow.cpp
 *        Created: 07/22/2015 03:16:57 AM
 *  Last Modified: 08/10/2017 18:21:45
 *
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#include "platforms.hpp"
#include "mainwindow.hpp"
#include "previewwindow.hpp"
#include "wilimagepackage.hpp"

PreviewWindow::PreviewWindow(int W, int H)
	: Fl_Double_Window(0, 0, W, H)
    , m_Inited(false)
    , m_ImageIndex(0)
    , m_Image(nullptr)
    , m_Buf()
{}

PreviewWindow::~PreviewWindow()
{
    delete m_Image;
}

void PreviewWindow::draw()
{
	Fl_Double_Window::draw();
    fl_rectf(0, 0, w(), h(), 0, 0, 0);

    extern WilImagePackage  g_WilPackage;
    extern MainWindow      *g_MainWindow;

    if(!m_Inited || (m_Inited && m_ImageIndex != g_MainWindow->SelectedImageIndex())){
        LoadImage();
    }

    if(m_Inited && m_ImageIndex == g_MainWindow->SelectedImageIndex() && m_Image){

        // for howto use make_current(), see my understanding as comments in:
        //      tools/mapeditor/src/animationpreviewarea.cpp
        // put this here as unchanged since it works properly
        if(PlatformWindows() && !PlatformLinux()){
            // wtf
            make_current();
        }

        int nX = (w() - m_Image->w()) / 2;
        int nY = (h() - m_Image->h()) / 2;
        int nW = m_Image->w();
        int nH = m_Image->h();

        m_Image->draw(nX, nY, nW, nH);
        auto stColor = fl_color();
        fl_color(FL_RED);
        fl_rect(nX, nY, nW, nH);
        fl_color(stColor);
    }
}

void PreviewWindow::LoadImage()
{
    delete m_Image; m_Image = nullptr;

    extern WilImagePackage  g_WilPackage;
    extern MainWindow      *g_MainWindow;

    if(true
            && g_WilPackage.SetIndex(g_MainWindow->SelectedImageIndex())
            && g_WilPackage.CurrentImageValid()){

        auto nW = g_WilPackage.CurrentImageInfo().shWidth;
        auto nH = g_WilPackage.CurrentImageInfo().shHeight;

        m_Buf.resize(0);
        m_Buf.resize(nW * nH);

        g_WilPackage.Decode(&(m_Buf[0]), 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF);
        m_Image = new Fl_RGB_Image((uchar *)(&(m_Buf[0])), nW, nH, 4);
    }

    m_Inited = false;
    if(m_Image){
        m_Inited = true;
        m_ImageIndex = g_MainWindow->SelectedImageIndex();
    }
}
