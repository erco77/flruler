//
// flruler -- draw an onscreen ruler for GUI design measurements
//
//   Version 1.05
//   Copyright (C) 2006,2020 Greg Ercolano
//
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
//   02111-1307 USA
//
//   To report bugs, create an 'issue' on the project's github page:
//   https://github.com/erco77/flruler
//

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <stdlib.h>	// exit
#include <stdio.h>	// sprintf

#if !defined(__APPLE__) && !defined(_WIN32)
#include <unistd.h>	// fork
#endif

#include "flruler.H"

#ifdef __APPLE__
// OSX
#define XSIZE 100		// XXX: 100px min window size OSX w/fltk 1.1.7
#define YSIZE 53
#define FONTSIZE 9		// 8 too small on osx
#else
// NON-OSX
#define XSIZE 67
#define YSIZE 53
#define FONTSIZE 8		// nice on linux
#endif

// Ruler widget
class RulerWindow : public Fl_Double_Window {
    int horizontal, reverse;
    int xsize, ysize;
    // DRAW A LINE AT X/Y
    //    Obeys 'reverse'
    //
    void Line(int X1,int Y1,int X2,int Y2) {
        if ( reverse ) {
	    X1 = w() - X1 - 1;
	    X2 = w() - X2 - 1;
	    Y1 = h() - Y1 - 1;
	    Y2 = h() - Y2 - 1;
	}
	fl_line(X1,Y1,X2,Y2);
    }
    // DRAW TEXT CENTERED AT X/Y
    //    Obeys 'reverse'
    //
    void TextCenter(const char *s,int X,int Y) {
        if ( reverse && horizontal ) {
	    X = w()-X-1;
	} else if ( reverse && ! horizontal ) {
	    Y = h()-Y-1;
	}
	// CENTER TEXT
	X = X - ((int)fl_width(s) / 2);
	Y = Y + ((int)fl_height() / 2) - 1;
	fl_draw(s,X,Y);
    }
    // FLTK: DRAW THE SCREEN
    void draw() {
        int RIGHT = w()-1;
	int BOTTOM = h()-1;
        char s[80];
        fl_color(133);		// ORANGE, 132, 92
        fl_rectf(0,0,w(),h());
        fl_color(FL_BLACK);
	fl_font(FL_HELVETICA, FONTSIZE);
	if ( horizontal ) {
	    // Draw horizontal ruler markings
	    for ( int X=0; X<=w(); X+=2 ) {
	        if ( X % 10 == 0 ) {
		    if ( X % 50 == 0 ) {
			Line(X,0,X,20);
			Line(X,BOTTOM-20,X,BOTTOM);
		    } else {
			Line(X,0,X,10);
			Line(X,BOTTOM-10,X,BOTTOM);
			sprintf(s,"%d",(X % 100) / 10);
			TextCenter(s,X,15);
			TextCenter(s,X,BOTTOM-15-1);
		    }
		} else {
	            Line(X,0,X,5);
		    Line(X,BOTTOM-5,X,BOTTOM);
		}
		if ( X % 50 == 0 ) {
		    sprintf(s,"%d",X);
		    TextCenter(s,X,BOTTOM/2);
		}
	    }
	} else {
	    // Draw vertical ruler markings
	    for ( int Y=0; Y<=h(); Y+=2 ) {
	        if ( Y % 10 == 0 ) {
		    if ( Y % 50 == 0 ) {
			Line(0,Y,20,Y);
			Line(RIGHT-20,Y,RIGHT,Y);
		    } else {
			Line(0,Y,10,Y);
			Line(RIGHT-10,Y,BOTTOM,Y);
			sprintf(s,"%d",(Y % 100) / 10);
			TextCenter(s,15,Y);
			TextCenter(s,RIGHT-15-1,Y);
		    }
		} else {
	            Line(0,Y,5,Y);
		    Line(RIGHT-5,Y,RIGHT,Y);
		}
		if ( Y % 50 == 0 ) {
		    sprintf(s,"%d",Y);
		    TextCenter(s,RIGHT/2,Y);
		}
	    }
	}
    }
    // FLTK: EVENT HANDLER
    int handle(int e);

public:
    // CHANGE HORIZONTAL ORIENTATION
    //     val: 1=horiz, 0=vertical
    //     rx: recommended x position for new orientation
    //     ry: recommended y position for new orientation
    //
    void Horizontal(int val, int rx=-1, int ry=-1) {
        horizontal = val;
	if ( horizontal ) {
	    if ( rx == -1 ) rx = 0;
	    if ( ry == -1 ) ry = Fl::event_y_root();
	    resize(rx, ry, xsize, YSIZE);
	} else {
	    if ( rx == -1 ) rx = Fl::event_x_root();
	    if ( ry == -1 ) ry = 0;
	    resize(rx, ry, XSIZE, ysize);
	}
	redraw();
    }
    // CHANGE FORWARD/REVERSE ORIENTATION
    void Reverse(int val) {
        reverse = val;
	redraw();
    }
    // CTOR
    RulerWindow(int W,int H,int hval,int rval):Fl_Double_Window(W,H,0) {
	end();
	resizable(this);
	border(0);
	show();
	xsize = Fl::w();
	ysize = Fl::h();
        Horizontal(hval);
	Reverse(rval);
    }
    // CTOR
    RulerWindow(int X,int Y,int W,int H,int hval, int rval):Fl_Double_Window(X,Y,W,H,0) {
	end();
	resizable(this);
	border(0);
	show();
	xsize = Fl::w();
	ysize = Fl::h();
        Horizontal(hval);
	Reverse(rval);
    }
};

// FLTK: EVENT HANDLER
int RulerWindow::handle(int e) {
    static int xoff = 0, yoff = 0, drag = 0;
    int ret = Fl_Double_Window::handle(e);
    switch ( e ) {
	case FL_KEYBOARD:
	case FL_PUSH:
	    drag = 0;
	    switch ( Fl::event_key() ) {
		// PREPARE FOR WINDOW DRAG
	        case FL_Button+FL_LEFT_MOUSE:
		    switch ( Fl::event_clicks() ) {
		        case 1:		// invert dir
		        case 3:
			    Reverse(reverse ^ 1);
			    break;
		        case 2:		// invert horiz/vert
			    Horizontal(horizontal ^ 1);
			    Reverse(0);
			    break;
		        case 4:
			    Horizontal(horizontal ^ 1);
			    Reverse(1);
			    break;
		    }
		    xoff = x() - Fl::event_x_root();
		    yoff = y() - Fl::event_y_root();
		    drag = 1;
		    break;

		// COPY RULER
	        case FL_Button+FL_MIDDLE_MOUSE:
		case 'c':
		case 'C':
		    new RulerWindow(Fl::event_x_root(),Fl::event_y_root(),0,0,horizontal,reverse);
		    return(1);
		// PUT ZERO AT LEFT
		case FL_Left:
		    Horizontal(1);
		    Reverse(0);
		    return(1);
		// PUT ZERO AT RIGHT
		case FL_Right:
		    Horizontal(1);
		    Reverse(1);
		    return(1);
		// PUT ZERO AT TOP
		case FL_Up:
		    Horizontal(0);
		    Reverse(0);
		    return(1);
		// PUT ZERO AT BOTTOM
		case FL_Down:
		    Horizontal(0);
		    Reverse(1);
		    return(1);
		// RESIZE ruler window
		case '-':		// shrink
		case 65453:		// numeric keypad's '-' key
		    if (horizontal) {
			xsize = xsize * 9 / 10;
			if (xsize < 150) xsize = 150;
			size(xsize,YSIZE);
		    } else {
			ysize = ysize * 9 / 10;
			if (ysize < 150) ysize = 150;
			size(XSIZE,ysize);
		    }
		    return(1);
		case '+':		// enlarge
		case '=':
		case 65451:		// numeric keypad's '+' key
		    if (horizontal) {
			xsize = xsize * 10 / 9;
			if (xsize > Fl::w()) xsize = Fl::w();
			size(xsize,YSIZE);
		    } else {
			ysize = ysize * 10 / 9;
			if (ysize > Fl::h()) ysize = Fl::h();
			size(XSIZE,ysize);
		    }
		    return(1);
		// QUIT
		case FL_Escape:		// fltk close window
		case FL_CTRL+'x':	// cut
		case FL_CTRL+'w':	// close window
		    hide();
		    return(1);
		// QUIT APPLICATION
		case 'q':
		case FL_CTRL+'q':
		case FL_ALT+'q':
		    exit(1);
	    }
	    ret = 1;
	    break;
	case FL_DRAG:
	    if ( drag ) {
		// LET USER DRAG WINDOW AROUND THE SCREEN
		position(xoff + Fl::event_x_root(), yoff + Fl::event_y_root());
		redraw();
	    }
	    ret = 1;
	    break;
	case FL_RELEASE:
	    ret = 1;
	    drag = 0;
	    break;
    }
    return(ret);
}

// FORK APP OFF AS CHILD PROCESS
//    Doesn't work in OSX or WIN32
void Fork() {
#if !defined(__APPLE__) && !defined(_WIN32)
    if ( fork() > 0 ) exit(0);
#endif
}

/// MAIN
int main(int argc, char *argv[]) {
    for ( int t=1; t<argc; t++ ) {
        if ( argv[t][0] == '-' ) {
	    switch ( argv[t][1] ) {
	        case 'h':
		    fprintf(stderr, "%s", G_help);
		    exit(1);
	    }
	}
    }
    Fork();
    int w = Fl::w();
    int h = YSIZE;
    RulerWindow win(w,h,1,0);
    return(Fl::run());
}
