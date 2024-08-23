QT       += core gui openglwidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    desktopview.cc \
    main.cc    \
    ../../brick_game/controller/controller.cc \
    ../../brick_game/snake/backend/snake_model.cc \
    ../../brick_game/tetris/tetris_model.cc \
    ../../brick_game/tetris/helpers/helpers.c \
    ../../brick_game/tetris/fsm/fsm_matrix.c \
    ../../brick_game/tetris/record/record_handling.c \
    ../../brick_game/tetris/score/score.c \
    ../../brick_game/tetris/tetromino/tetromino.c


HEADERS += \
    desktopview.h \
    ../../brick_game/controller/controller.h \
    ../../brick_game/base/base_model.h \
    ../../brick_game/snake/backend/snake_model.h \
    ../../brick_game/snake/data/data.h \
    ../../brick_game/tetris/tetris_model.h \
    ../../brick_game/tetris/tetris_backend.h \
    ../../brick_game/tetris/helpers/helpers.h \
    ../../brick_game/tetris/fsm/fsm.h \
    ../../brick_game/tetris/record/record_handling.h \
    ../../brick_game/tetris/score/score.h \
    ../../brick_game/tetris/tetromino/tetromino.h
    

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
