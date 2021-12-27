#pragma once

#include "gl_scene_types.h"
#include "gl_scene_camera.h"
#include "gl_scene_mesh.h"

namespace gl_scene
{

namespace defaults
{

namespace common
{
extern const int kSelectionMask;
}  // namespace common

namespace cameras
{
extern const Camera kDefault;
}  // namespace cameras

namespace colors
{
extern const Color kSceneBackground;
extern const Color kSelectedItem;
extern const Color kHoveredItem;
extern const Color kSelectionRect;
extern const Color kDefault;
}  // namespace colors

namespace lights
{
extern const Light kDefault;
}  // namespace lights

namespace meshes
{

namespace id
{
extern const MeshID kGrid;
extern const MeshID kCube;
extern const MeshID kConus;
extern const MeshID kCylinder;
extern const MeshID kSphere;
extern const MeshID kOktaeder;
}  // namespace id

extern const Mesh kGrid;
extern const Mesh kCube;
extern const Mesh kConus;
extern const Mesh kCylinder;
extern const Mesh kSphere;
extern const Mesh kOktaeder;
extern const Mesh::Map kDefault;
}  // namespace meshes

namespace pipes
{

namespace id
{
extern const PipeID k2D;
extern const PipeID k3D;
extern const PipeID k3DTextured;
extern const PipeID k2DTextured;
extern const PipeID kTexturedFlat;
extern const PipeID kTexturedFlatLast;
extern const PipeID kSelection;
extern const PipeID k2DLast;
extern const PipeID k3DLast;
extern const PipeID k3DTexturedLast;
extern const PipeID k2DTexturedLast;
}  // namespace id

}  // namespace pipes

namespace shaders
{
extern const Shader::Map kDefault;
}  // namespace shaders

namespace textures
{

namespace id
{

extern const TextureID kCommon;
}  // namespace id

extern const TexturesMap kDefault;
}  // namespace textures

}  // namespace defaults

}  // namespace gl_scene
