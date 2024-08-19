QT       += core gui openglwidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    desktopview.cpp \
    main.cpp    \
    ../main/Controller.cpp \
    ../main/Model.cpp \
    ../main/TetrisModel.cpp \
    ../tetris/helpers/helpers.c \
    ../tetris/fsm/fsm_matrix.c \
    ../tetris/record/record_handling.c \
    ../tetris/score/score.c \
    ../tetris/tetromino/tetromino.c


HEADERS += \
    desktopview.h \
    ../include/baseModel.hpp \
    ../include/Controller.hpp \
    ../include/data.hpp \
    ../include/Model.hpp \
    ../include/TetrisModel.hpp \
    ../tetris/*.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
