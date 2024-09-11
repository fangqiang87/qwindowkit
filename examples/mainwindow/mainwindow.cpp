// Copyright (C) 2023-2024 Stdware Collections (https://www.github.com/stdware)
// Copyright (C) 2021-2023 wangwenx190 (Yuhang Zhao)
// SPDX-License-Identifier: Apache-2.0

#include "mainwindow.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyle>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#  include <QtGui/QActionGroup>
#else
#  include <QtWidgets/QActionGroup>
#endif

// #include <QtWebEngineWidgets/QWebEngineView>

#include <QWKWidgets/widgetwindowagent.h>

#include <widgetframe/windowbar.h>
#include <widgetframe/windowbutton.h>

#include <Windows.h>

class ClockWidget : public QLabel {
public:
    explicit ClockWidget(QWidget *parent = nullptr) : QLabel(parent) {
        startTimer(100);
        setAlignment(Qt::AlignCenter);
    }

    ~ClockWidget() override = default;

protected:
    void timerEvent(QTimerEvent *event) override {
        setText(QTime::currentTime().toString(QStringLiteral("hh:mm:ss")));
    }

    void mouseReleaseEvent(QMouseEvent *ev) override {
        QLabel::mouseReleaseEvent(ev);

        setAttribute(Qt::WA_DontCreateNativeAncestors);
        qDebug() << winId();
    }
};

static QHash<int, const wchar_t *> xmsgHash = //
    {
        //
        {0,    L"WM_NULL"                  },
        {1,    L"WM_CREATE"                },
        {2,    L"WM_DESTROY"               },
        {3,    L"WM_MOVE"                  },
        {5,    L"WM_SIZE"                  },
        {6,    L"WM_ACTIVATE"              },
        {7,    L"WM_SETFOCUS"              },
        {8,    L"WM_KILLFOCUS"             },
        {10,   L"WM_ENABLE"                },
        {11,   L"WM_SETREDRAW"             },
        {12,   L"WM_SETTEXT"               },
        {13,   L"WM_GETTEXT"               },
        {14,   L"WM_GETTEXTLENGTH"         },
        {15,   L"WM_PAINT"                 },
        {16,   L"WM_CLOSE"                 },
        {17,   L"WM_QUERYENDSESSION"       },
        {18,   L"WM_QUIT"                  },
        {19,   L"WM_QUERYOPEN"             },
        {20,   L"WM_ERASEBKGND"            },
        {21,   L"WM_SYSCOLORCHANGE"        },
        {22,   L"WM_ENDSESSION"            },
        {24,   L"WM_SHOWWINDOW"            },
        {25,   L"WM_CTLCOLOR"              },
        {26,   L"WM_WININICHANGE"          },
        {27,   L"WM_DEVMODECHANGE"         },
        {28,   L"WM_ACTIVATEAPP"           },
        {29,   L"WM_FONTCHANGE"            },
        {30,   L"WM_TIMECHANGE"            },
        {31,   L"WM_CANCELMODE"            },
        {32,   L"WM_SETCURSOR"             },
        {33,   L"WM_MOUSEACTIVATE"         },
        {34,   L"WM_CHILDACTIVATE"         },
        {35,   L"WM_QUEUESYNC"             },
        {36,   L"WM_GETMINMAXINFO"         },
        {38,   L"WM_PAINTICON"             },
        {39,   L"WM_ICONERASEBKGND"        },
        {40,   L"WM_NEXTDLGCTL"            },
        {42,   L"WM_SPOOLERSTATUS"         },
        {43,   L"WM_DRAWITEM"              },
        {44,   L"WM_MEASUREITEM"           },
        {45,   L"WM_DELETEITEM"            },
        {46,   L"WM_VKEYTOITEM"            },
        {47,   L"WM_CHARTOITEM"            },
        {48,   L"WM_SETFONT"               },
        {49,   L"WM_GETFONT"               },
        {50,   L"WM_SETHOTKEY"             },
        {51,   L"WM_GETHOTKEY"             },
        {55,   L"WM_QUERYDRAGICON"         },
        {57,   L"WM_COMPAREITEM"           },
        {61,   L"WM_GETOBJECT"             },
        {65,   L"WM_COMPACTING"            },
        {68,   L"WM_COMMNOTIFY"            },
        {70,   L"WM_WINDOWPOSCHANGING"     },
        {71,   L"WM_WINDOWPOSCHANGED"      },
        {72,   L"WM_POWER"                 },
        {73,   L"WM_COPYGLOBALDATA"        },
        {74,   L"WM_COPYDATA"              },
        {75,   L"WM_CANCELJOURNAL"         },
        {78,   L"WM_NOTIFY"                },
        {80,   L"WM_INPUTLANGCHANGEREQUEST"},
        {81,   L"WM_INPUTLANGCHANGE"       },
        {82,   L"WM_TCARD"                 },
        {83,   L"WM_HELP"                  },
        {84,   L"WM_USERCHANGED"           },
        {85,   L"WM_NOTIFYFORMAT"          },
        {123,  L"WM_CONTEXTMENU"           },
        {124,  L"WM_STYLECHANGING"         },
        {125,  L"WM_STYLECHANGED"          },
        {126,  L"WM_DISPLAYCHANGE"         },
        {127,  L"WM_GETICON"               },
        {128,  L"WM_SETICON"               },
        {129,  L"WM_NCCREATE"              },
        {130,  L"WM_NCDESTROY"             },
        {131,  L"WM_NCCALCSIZE"            },
        {132,  L"WM_NCHITTEST"             },
        {133,  L"WM_NCPAINT"               },
        {134,  L"WM_NCACTIVATE"            },
        {135,  L"WM_GETDLGCODE"            },
        {136,  L"WM_SYNCPAINT"             },
        {160,  L"WM_NCMOUSEMOVE"           },
        {161,  L"WM_NCLBUTTONDOWN"         },
        {162,  L"WM_NCLBUTTONUP"           },
        {163,  L"WM_NCLBUTTONDBLCLK"       },
        {164,  L"WM_NCRBUTTONDOWN"         },
        {165,  L"WM_NCRBUTTONUP"           },
        {166,  L"WM_NCRBUTTONDBLCLK"       },
        {167,  L"WM_NCMBUTTONDOWN"         },
        {168,  L"WM_NCMBUTTONUP"           },
        {169,  L"WM_NCMBUTTONDBLCLK"       },
        {171,  L"WM_NCXBUTTONDOWN"         },
        {172,  L"WM_NCXBUTTONUP"           },
        {173,  L"WM_NCXBUTTONDBLCLK"       },
        {176,  L"EM_GETSEL"                },
        {177,  L"EM_SETSEL"                },
        {178,  L"EM_GETRECT"               },
        {179,  L"EM_SETRECT"               },
        {180,  L"EM_SETRECTNP"             },
        {181,  L"EM_SCROLL"                },
        {182,  L"EM_LINESCROLL"            },
        {183,  L"EM_SCROLLCARET"           },
        {185,  L"EM_GETMODIFY"             },
        {187,  L"EM_SETMODIFY"             },
        {188,  L"EM_GETLINECOUNT"          },
        {189,  L"EM_LINEINDEX"             },
        {190,  L"EM_SETHANDLE"             },
        {191,  L"EM_GETHANDLE"             },
        {192,  L"EM_GETTHUMB"              },
        {193,  L"EM_LINELENGTH"            },
        {194,  L"EM_REPLACESEL"            },
        {195,  L"EM_SETFONT"               },
        {196,  L"EM_GETLINE"               },
        {197,  L"EM_LIMITTEXT"             },
        {197,  L"EM_SETLIMITTEXT"          },
        {198,  L"EM_CANUNDO"               },
        {199,  L"EM_UNDO"                  },
        {200,  L"EM_FMTLINES"              },
        {201,  L"EM_LINEFROMCHAR"          },
        {202,  L"EM_SETWORDBREAK"          },
        {203,  L"EM_SETTABSTOPS"           },
        {204,  L"EM_SETPASSWORDCHAR"       },
        {205,  L"EM_EMPTYUNDOBUFFER"       },
        {206,  L"EM_GETFIRSTVISIBLELINE"   },
        {207,  L"EM_SETREADONLY"           },
        {209,  L"EM_SETWORDBREAKPROC"      },
        {209,  L"EM_GETWORDBREAKPROC"      },
        {210,  L"EM_GETPASSWORDCHAR"       },
        {211,  L"EM_SETMARGINS"            },
        {212,  L"EM_GETMARGINS"            },
        {213,  L"EM_GETLIMITTEXT"          },
        {214,  L"EM_POSFROMCHAR"           },
        {215,  L"EM_CHARFROMPOS"           },
        {216,  L"EM_SETIMESTATUS"          },
        {217,  L"EM_GETIMESTATUS"          },
        {224,  L"SBM_SETPOS"               },
        {225,  L"SBM_GETPOS"               },
        {226,  L"SBM_SETRANGE"             },
        {227,  L"SBM_GETRANGE"             },
        {228,  L"SBM_ENABLE_ARROWS"        },
        {230,  L"SBM_SETRANGEREDRAW"       },
        {233,  L"SBM_SETSCROLLINFO"        },
        {234,  L"SBM_GETSCROLLINFO"        },
        {235,  L"SBM_GETSCROLLBARINFO"     },
        {240,  L"BM_GETCHECK"              },
        {241,  L"BM_SETCHECK"              },
        {242,  L"BM_GETSTATE"              },
        {243,  L"BM_SETSTATE"              },
        {244,  L"BM_SETSTYLE"              },
        {245,  L"BM_CLICK"                 },
        {246,  L"BM_GETIMAGE"              },
        {247,  L"BM_SETIMAGE"              },
        {248,  L"BM_SETDONTCLICK"          },
        {255,  L"WM_INPUT"                 },
        {256,  L"WM_KEYDOWN"               },
        {256,  L"WM_KEYFIRST"              },
        {257,  L"WM_KEYUP"                 },
        {258,  L"WM_CHAR"                  },
        {259,  L"WM_DEADCHAR"              },
        {260,  L"WM_SYSKEYDOWN"            },
        {261,  L"WM_SYSKEYUP"              },
        {262,  L"WM_SYSCHAR"               },
        {263,  L"WM_SYSDEADCHAR"           },
        {264,  L"WM_KEYLAST"               },
        {265,  L"WM_UNICHAR"               },
        {265,  L"WM_WNT_CONVERTREQUESTEX"  },
        {266,  L"WM_CONVERTREQUEST"        },
        {267,  L"WM_CONVERTRESULT"         },
        {268,  L"WM_INTERIM"               },
        {269,  L"WM_IME_STARTCOMPOSITION"  },
        {270,  L"WM_IME_ENDCOMPOSITION"    },
        {271,  L"WM_IME_COMPOSITION"       },
        {271,  L"WM_IME_KEYLAST"           },
        {272,  L"WM_INITDIALOG"            },
        {273,  L"WM_COMMAND"               },
        {274,  L"WM_SYSCOMMAND"            },
        {275,  L"WM_TIMER"                 },
        {276,  L"WM_HSCROLL"               },
        {277,  L"WM_VSCROLL"               },
        {278,  L"WM_INITMENU"              },
        {279,  L"WM_INITMENUPOPUP"         },
        {280,  L"WM_SYSTIMER"              },
        {287,  L"WM_MENUSELECT"            },
        {288,  L"WM_MENUCHAR"              },
        {289,  L"WM_ENTERIDLE"             },
        {290,  L"WM_MENURBUTTONUP"         },
        {291,  L"WM_MENUDRAG"              },
        {292,  L"WM_MENUGETOBJECT"         },
        {293,  L"WM_UNINITMENUPOPUP"       },
        {294,  L"WM_MENUCOMMAND"           },
        {295,  L"WM_CHANGEUISTATE"         },
        {296,  L"WM_UPDATEUISTATE"         },
        {297,  L"WM_QUERYUISTATE"          },
        {306,  L"WM_CTLCOLORMSGBOX"        },
        {307,  L"WM_CTLCOLOREDIT"          },
        {308,  L"WM_CTLCOLORLISTBOX"       },
        {309,  L"WM_CTLCOLORBTN"           },
        {310,  L"WM_CTLCOLORDLG"           },
        {311,  L"WM_CTLCOLORSCROLLBAR"     },
        {312,  L"WM_CTLCOLORSTATIC"        },
        {512,  L"WM_MOUSEFIRST"            },
        {512,  L"WM_MOUSEMOVE"             },
        {513,  L"WM_LBUTTONDOWN"           },
        {514,  L"WM_LBUTTONUP"             },
        {515,  L"WM_LBUTTONDBLCLK"         },
        {516,  L"WM_RBUTTONDOWN"           },
        {517,  L"WM_RBUTTONUP"             },
        {518,  L"WM_RBUTTONDBLCLK"         },
        {519,  L"WM_MBUTTONDOWN"           },
        {520,  L"WM_MBUTTONUP"             },
        {521,  L"WM_MBUTTONDBLCLK"         },
        {521,  L"WM_MOUSELAST"             },
        {522,  L"WM_MOUSEWHEEL"            },
        {523,  L"WM_XBUTTONDOWN"           },
        {524,  L"WM_XBUTTONUP"             },
        {525,  L"WM_XBUTTONDBLCLK"         },
        {528,  L"WM_PARENTNOTIFY"          },
        {529,  L"WM_ENTERMENULOOP"         },
        {530,  L"WM_EXITMENULOOP"          },
        {531,  L"WM_NEXTMENU"              },
        {532,  L"WM_SIZING"                },
        {533,  L"WM_CAPTURECHANGED"        },
        {534,  L"WM_MOVING"                },
        {536,  L"WM_POWERBROADCAST"        },
        {537,  L"WM_DEVICECHANGE"          },
        {544,  L"WM_MDICREATE"             },
        {545,  L"WM_MDIDESTROY"            },
        {546,  L"WM_MDIACTIVATE"           },
        {547,  L"WM_MDIRESTORE"            },
        {548,  L"WM_MDINEXT"               },
        {549,  L"WM_MDIMAXIMIZE"           },
        {550,  L"WM_MDITILE"               },
        {551,  L"WM_MDICASCADE"            },
        {552,  L"WM_MDIICONARRANGE"        },
        {553,  L"WM_MDIGETACTIVE"          },
        {560,  L"WM_MDISETMENU"            },
        {561,  L"WM_ENTERSIZEMOVE"         },
        {562,  L"WM_EXITSIZEMOVE"          },
        {563,  L"WM_DROPFILES"             },
        {564,  L"WM_MDIREFRESHMENU"        },
        {640,  L"WM_IME_REPORT"            },
        {641,  L"WM_IME_SETCONTEXT"        },
        {642,  L"WM_IME_NOTIFY"            },
        {643,  L"WM_IME_CONTROL"           },
        {644,  L"WM_IME_COMPOSITIONFULL"   },
        {645,  L"WM_IME_SELECT"            },
        {646,  L"WM_IME_CHAR"              },
        {648,  L"WM_IME_REQUEST"           },
        {656,  L"WM_IMEKEYDOWN"            },
        {656,  L"WM_IME_KEYDOWN"           },
        {657,  L"WM_IMEKEYUP"              },
        {657,  L"WM_IME_KEYUP"             },
        {672,  L"WM_NCMOUSEHOVER"          },
        {673,  L"WM_MOUSEHOVER"            },
        {674,  L"WM_NCMOUSELEAVE"          },
        {675,  L"WM_MOUSELEAVE"            },
        {768,  L"WM_CUT"                   },
        {769,  L"WM_COPY"                  },
        {770,  L"WM_PASTE"                 },
        {771,  L"WM_CLEAR"                 },
        {772,  L"WM_UNDO"                  },
        {773,  L"WM_RENDERFORMAT"          },
        {774,  L"WM_RENDERALLFORMATS"      },
        {775,  L"WM_DESTROYCLIPBOARD"      },
        {776,  L"WM_DRAWCLIPBOARD"         },
        {777,  L"WM_PAINTCLIPBOARD"        },
        {778,  L"WM_VSCROLLCLIPBOARD"      },
        {779,  L"WM_SIZECLIPBOARD"         },
        {780,  L"WM_ASKCBFORMATNAME"       },
        {781,  L"WM_CHANGECBCHAIN"         },
        {782,  L"WM_HSCROLLCLIPBOARD"      },
        {783,  L"WM_QUERYNEWPALETTE"       },
        {784,  L"WM_PALETTEISCHANGING"     },
        {785,  L"WM_PALETTECHANGED"        },
        {786,  L"WM_HOTKEY"                },
        {791,  L"WM_PRINT"                 },
        {792,  L"WM_PRINTCLIENT"           },
        {793,  L"WM_APPCOMMAND"            },
        {856,  L"WM_HANDHELDFIRST"         },
        {863,  L"WM_HANDHELDLAST"          },
        {864,  L"WM_AFXFIRST"              },
        {895,  L"WM_AFXLAST"               },
        {896,  L"WM_PENWINFIRST"           },
        {897,  L"WM_RCRESULT"              },
        {898,  L"WM_HOOKRCRESULT"          },
        {899,  L"WM_GLOBALRCCHANGE"        },
        {899,  L"WM_PENMISCINFO"           },
        {900,  L"WM_SKB"                   },
        {901,  L"WM_HEDITCTL"              },
        {901,  L"WM_PENCTL"                },
        {902,  L"WM_PENMISC"               },
        {903,  L"WM_CTLINIT"               },
        {904,  L"WM_PENEVENT"              },
        {911,  L"WM_PENWINLAST"            },
        {1024, L"WM_USER"                  },
};

class MyPushButton : public QPushButton {
public:
    MyPushButton(const QString &text, QWidget *parent = nullptr) : QPushButton(text, parent) {
    }

protected:
    virtual bool nativeEvent(const QByteArray &eventType, void *message,
                             QT_NATIVE_EVENT_RESULT_TYPE *result) override {

        auto msg = reinterpret_cast<MSG *>(message);
        auto s = QString::fromStdWString(xmsgHash.value(msg->message, L""));
        messages.insert(s, 0);
        // qDebug().noquote() << s;

        if (msg->message == WM_PAINT) {
            auto hwnd = reinterpret_cast<HWND>(winId());
            RECT r;
            GetWindowRect(reinterpret_cast<HWND>(winId()), &r);

            POINT point;
            point.x = r.left;
            point.y = r.top;

            auto orgPoint = point;

            HWND hwndParent = reinterpret_cast<HWND>(window()->winId());
            if (hwndParent) {
                ScreenToClient(hwndParent, &point);
            }

            // qDebug() << QPoint(orgPoint.x, orgPoint.y) << QPoint(point.x, point.y) << pos();
        }

        if (msg->message == WM_NCCALCSIZE) {
            qDebug() << QDateTime::currentDateTime() << "WM_NCCALCSIZE";
        }

        // switch (msg->message) {
        //     case WM_WINDOWPOSCHANGING: {
        //         // ### FIXME: How does this problem happen and why is it solved?
        //         // When toggling the "Show theme color in title bar and window border" setting in
        //         // Windows Settings, or calling `DrawMenuBar()`, Windows sends a message of
        //         // WM_WINDOWPOSCHANGING with flags 0x37. If we do not process this message,
        //         // the client area as a whole will shift to the left, which looks very abnormal
        //         if
        //         // we don't repaint it. This exception disappears if we add SWP_NOCOPYBITS flag.
        //         // But I don't know what caused the problem, or why this would solve it.
        //         static constexpr const auto kBadWindowPosFlag =
        //             SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED;
        //         const auto windowPos = reinterpret_cast<LPWINDOWPOS>(msg->lParam);
        //         if (windowPos->flags == kBadWindowPosFlag) {
        //             windowPos->flags |= SWP_NOCOPYBITS;
        //         }
        //         break;
        //     }
        //     default:
        //         break;
        // }

        return false;
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        QPushButton::mouseReleaseEvent(event);
        if (!native) {
            setAttribute(Qt::WA_DontCreateNativeAncestors);
            qDebug() << winId();
            native = true;
            return;
        }
    }

    void resizeEvent(QResizeEvent *event) override {
        // qDebug() << "Resize:" << event->oldSize() << event->size();
        QPushButton::resizeEvent(event);
    }

    void moveEvent(QMoveEvent *event) override {
        qDebug() << "Move:" << event->oldPos() << event->pos();
        QPushButton::moveEvent(event);
    }

    bool native = false;
    QMap<QString, int> messages;
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    installWindowAgent();

#if 1
    // auto clockWidget = new ClockWidget();
    // clockWidget->setObjectName(QStringLiteral("clock-widget"));
    // clockWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // setCentralWidget(clockWidget);

    auto layout = new QGridLayout();
    auto btn1 = new QPushButton("1");
    btn1->setProperty("test-button", true);
    auto btn2 = new QPushButton("2");
    btn2->setProperty("test-button", true);
    auto btn3 = new QPushButton("3");
    btn3->setProperty("test-button", true);
    auto btn4 = new MyPushButton("4");
    btn4->setProperty("test-button", true);
    btn4->setFixedHeight(31);
    layout->addWidget(btn1, 0, 0);
    layout->addWidget(btn2, 0, 1);
    layout->addWidget(btn3, 1, 0);
    layout->addWidget(btn4, 1, 1);
    auto widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);
#else
    auto webView = new QWebEngineView();
    webView->load(QUrl("https://www.baidu.com"));
    setCentralWidget(webView);
#endif

    loadStyleSheet(Dark);

    setWindowTitle(tr("Example MainWindow"));
    resize(800, 600);

    // setFixedHeight(600);
    // windowAgent->centralize();
}

static inline void emulateLeaveEvent(QWidget *widget) {
    Q_ASSERT(widget);
    if (!widget) {
        return;
    }
    QTimer::singleShot(0, widget, [widget]() {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        const QScreen *screen = widget->screen();
#else
        const QScreen *screen = widget->windowHandle()->screen();
#endif
        const QPoint globalPos = QCursor::pos(screen);
        if (!QRect(widget->mapToGlobal(QPoint{0, 0}), widget->size()).contains(globalPos)) {
            QCoreApplication::postEvent(widget, new QEvent(QEvent::Leave));
            if (widget->testAttribute(Qt::WA_Hover)) {
                const QPoint localPos = widget->mapFromGlobal(globalPos);
                const QPoint scenePos = widget->window()->mapFromGlobal(globalPos);
                static constexpr const auto oldPos = QPoint{};
                const Qt::KeyboardModifiers modifiers = QGuiApplication::keyboardModifiers();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))
                const auto event =
                    new QHoverEvent(QEvent::HoverLeave, scenePos, globalPos, oldPos, modifiers);
                Q_UNUSED(localPos);
#elif (QT_VERSION >= QT_VERSION_CHECK(6, 3, 0))
                const auto event =  new QHoverEvent(QEvent::HoverLeave, localPos, globalPos, oldPos, modifiers);
                Q_UNUSED(scenePos);
#else
                const auto event =  new QHoverEvent(QEvent::HoverLeave, localPos, oldPos, modifiers);
                Q_UNUSED(scenePos);
#endif
                QCoreApplication::postEvent(widget, event);
            }
        }
    });
}

MainWindow::~MainWindow() = default;

bool MainWindow::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::WindowActivate: {
            auto menu = menuWidget();
            if (menu) {
                menu->setProperty("bar-active", true);
                style()->polish(menu);
            }
            break;
        }

        case QEvent::WindowDeactivate: {
            auto menu = menuWidget();
            if (menu) {
                menu->setProperty("bar-active", false);
                style()->polish(menu);
            }
            break;
        }

        default:
            break;
    }
    return QMainWindow::event(event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // if (!(qApp->keyboardModifiers() & Qt::ControlModifier)) {
    //     QTimer::singleShot(1000, this, &QWidget::show);
    // }
    event->accept();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    // qDebug() << "MainWindow resized";
}

void MainWindow::installWindowAgent() {
    // 1. Setup window agent
    windowAgent = new QWK::WidgetWindowAgent(this);
    windowAgent->setup(this);

    // 2. Construct your title bar
    auto menuBar = [this]() {
        auto menuBar = new QMenuBar();

        // Virtual menu
        auto file = new QMenu(tr("File(&F)"), menuBar);
        file->addAction(new QAction(tr("New(&N)"), menuBar));
        file->addAction(new QAction(tr("Open(&O)"), menuBar));
        file->addSeparator();

        auto edit = new QMenu(tr("Edit(&E)"), menuBar);
        edit->addAction(new QAction(tr("Undo(&U)"), menuBar));
        edit->addAction(new QAction(tr("Redo(&R)"), menuBar));

        // Theme action
        auto darkAction = new QAction(tr("Enable dark theme"), menuBar);
        darkAction->setCheckable(true);
        connect(darkAction, &QAction::triggered, this, [this](bool checked) {
            loadStyleSheet(checked ? Dark : Light); //
        });
        connect(this, &MainWindow::themeChanged, darkAction, [this, darkAction]() {
            darkAction->setChecked(currentTheme == Dark); //
        });

#ifdef Q_OS_WIN
        auto dwmBlurAction = new QAction(tr("Enable DWM blur"), menuBar);
        dwmBlurAction->setCheckable(true);
        connect(dwmBlurAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("dwm-blur"), checked)) {
                return;
            }
            setProperty("custom-style", checked);
            style()->polish(this);
        });

        auto acrylicAction = new QAction(tr("Enable acrylic material"), menuBar);
        acrylicAction->setCheckable(true);
        connect(acrylicAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("acrylic-material"), true)) {
                return;
            }
            setProperty("custom-style", checked);
            style()->polish(this);
        });

        auto micaAction = new QAction(tr("Enable mica"), menuBar);
        micaAction->setCheckable(true);
        connect(micaAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("mica"), checked)) {
                return;
            }
            setProperty("custom-style", checked);
            style()->polish(this);
        });

        auto micaAltAction = new QAction(tr("Enable mica alt"), menuBar);
        micaAltAction->setCheckable(true);
        connect(micaAltAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("mica-alt"), checked)) {
                return;
            }
            setProperty("custom-style", checked);
            style()->polish(this);
        });
#elif defined(Q_OS_MAC)
        auto darkBlurAction = new QAction(tr("Dark blur"), menuBar);
        darkBlurAction->setCheckable(true);
        connect(darkBlurAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("blur-effect"), "dark")) {
                return;
            }
            if (checked) {
                setProperty("custom-style", true);
                style()->polish(this);
            }
        });

        auto lightBlurAction = new QAction(tr("Light blur"), menuBar);
        lightBlurAction->setCheckable(true);
        connect(lightBlurAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("blur-effect"), "light")) {
                return;
            }
            if (checked) {
                setProperty("custom-style", true);
                style()->polish(this);
            }
        });

        auto noBlurAction = new QAction(tr("No blur"), menuBar);
        noBlurAction->setCheckable(true);
        connect(noBlurAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("blur-effect"), "none")) {
                return;
            }
            if (checked) {
                setProperty("custom-style", false);
                style()->polish(this);
            }
        });

        auto macStyleGroup = new QActionGroup(menuBar);
        macStyleGroup->addAction(darkBlurAction);
        macStyleGroup->addAction(lightBlurAction);
        macStyleGroup->addAction(noBlurAction);
#endif

        // Real menu
        auto settings = new QMenu(tr("Settings(&S)"), menuBar);
        settings->addAction(darkAction);

#ifdef Q_OS_WIN
        settings->addSeparator();
        settings->addAction(dwmBlurAction);
        settings->addAction(acrylicAction);
        settings->addAction(micaAction);
        settings->addAction(micaAltAction);
#elif defined(Q_OS_MAC)
        settings->addAction(darkBlurAction);
        settings->addAction(lightBlurAction);
        settings->addAction(noBlurAction);
#endif

        menuBar->addMenu(file);
        menuBar->addMenu(edit);
        menuBar->addMenu(settings);
        return menuBar;
    }();
    menuBar->setObjectName(QStringLiteral("win-menu-bar"));

    auto titleLabel = new QLabel();
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName(QStringLiteral("win-title-label"));

#ifndef Q_OS_MAC
    auto iconButton = new QWK::WindowButton();
    iconButton->setObjectName(QStringLiteral("icon-button"));
    iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto minButton = new QWK::WindowButton();
    minButton->setObjectName(QStringLiteral("min-button"));
    minButton->setProperty("system-button", true);
    minButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto maxButton = new QWK::WindowButton();
    maxButton->setCheckable(true);
    maxButton->setObjectName(QStringLiteral("max-button"));
    maxButton->setProperty("system-button", true);
    maxButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto closeButton = new QWK::WindowButton();
    closeButton->setObjectName(QStringLiteral("close-button"));
    closeButton->setProperty("system-button", true);
    closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
#endif

    auto windowBar = new QWK::WindowBar();
#ifndef Q_OS_MAC
    windowBar->setIconButton(iconButton);
    windowBar->setMinButton(minButton);
    windowBar->setMaxButton(maxButton);
    windowBar->setCloseButton(closeButton);
#endif
    windowBar->setMenuBar(menuBar);
    windowBar->setTitleLabel(titleLabel);
    windowBar->setHostWidget(this);

    windowAgent->setTitleBar(windowBar);
#ifndef Q_OS_MAC
    windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, iconButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, minButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, maxButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Close, closeButton);
#endif
    windowAgent->setHitTestVisible(menuBar, true);

#ifdef Q_OS_MAC
    windowAgent->setSystemButtonAreaCallback([](const QSize &size) {
        static constexpr const int width = 75;
        return QRect(QPoint(size.width() - width, 0), QSize(width, size.height())); //
    });
#endif

    setMenuWidget(windowBar);


#ifndef Q_OS_MAC
    connect(windowBar, &QWK::WindowBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(windowBar, &QWK::WindowBar::maximizeRequested, this, [this, maxButton](bool max) {
        if (max) {
            showMaximized();
        } else {
            showNormal();
        }

        // It's a Qt issue that if a QAbstractButton::clicked triggers a window's maximization,
        // the button remains to be hovered until the mouse move. As a result, we need to
        // manually send leave events to the button.
        emulateLeaveEvent(maxButton);
    });
    connect(windowBar, &QWK::WindowBar::closeRequested, this, &QWidget::close);
#endif
}

void MainWindow::loadStyleSheet(Theme theme) {
    if (!styleSheet().isEmpty() && theme == currentTheme)
        return;
    currentTheme = theme;

    if (QFile qss(theme == Dark ? QStringLiteral(":/dark-style.qss")
                                : QStringLiteral(":/light-style.qss"));
        qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(qss.readAll()));
        Q_EMIT themeChanged();
    }
}
