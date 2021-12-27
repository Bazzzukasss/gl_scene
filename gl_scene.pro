QT -= gui
QT += opengl

TEMPLATE = lib
CONFIG += staticlib c++11
DESTDIR = ../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/gl_scene.cpp \
    src/gl_scene_camera.cpp \
    src/gl_scene_defaults.cpp \
    src/gl_scene_generator.cpp \
    src/gl_scene_glass.cpp \
    src/gl_scene_item.cpp \
    src/gl_scene_loader.cpp \
    src/gl_scene_manipulator.cpp \
    src/gl_scene_mesh.cpp \
    src/gl_scene_object.cpp \
    src/gl_scene_pipe.cpp \
    src/gl_scene_projection.cpp \
    src/gl_scene_utility.cpp \
    src/gl_scene_view.cpp

HEADERS += \
    inc/gl_scene.h \
    inc/gl_scene_camera.h \
    inc/gl_scene_defaults.h \
    inc/gl_scene_generator.h \
    inc/gl_scene_glass.h \
    inc/gl_scene_item.h \
    inc/gl_scene_loader.h \
    inc/gl_scene_manipulator.h \
    inc/gl_scene_mesh.h \
    inc/gl_scene_object.h \
    inc/gl_scene_pipe.h \
    inc/gl_scene_projection.h \
    inc/gl_scene_types.h \
    inc/gl_scene_utility.h \
    inc/gl_scene_view.h

INCLUDEPATH += inc

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
