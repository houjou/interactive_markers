/*
 * Copyright (c) 2010, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <rviz_renderer_ogre/simple_shape.h>
#include <rviz_renderer_ogre/transform_node.h>
#include <rviz_renderer_ogre/material.h>
#include <rviz_renderer_ogre/mesh_instance.h>
#include <rviz_renderer_ogre/mesh_loader.h>

#include <rviz_uuid/uuid.h>

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubEntity.h>
#include <OGRE/OgreSceneNode.h>

#include <ros/types.h>

namespace rviz_renderer_ogre
{

static const char* g_shape_meshes[] =
{
    //"package://"ROS_PACKAGE_NAME"/media/models/cone.mesh",
  "package://"ROS_PACKAGE_NAME"/media/models/cone.dae",
    "package://"ROS_PACKAGE_NAME"/media/models/cube.mesh",
    //"package://"ROS_PACKAGE_NAME"/media/models/cylinder.mesh",
    "package://"ROS_PACKAGE_NAME"/media/models/cylinder.dae",
    //"package://"ROS_PACKAGE_NAME"/media/models/sphere.mesh"
    "package://"ROS_PACKAGE_NAME"/media/models/sphere.dae"
};

SimpleShape::SimpleShape(Ogre::SceneManager* scene_manager, Type type, TransformNode* node)
: scene_manager_(scene_manager)
{
  loadMesh(g_shape_meshes[type]);
  inst_ = new MeshInstance(rviz_uuid::UUID::Generate(), scene_manager, node, g_shape_meshes[type]);
  material_.reset(new Material(rviz_uuid::UUID::Generate()));

  rviz_msgs::Material mat;
  mat.has_color = true;
  mat.opacity = 1.0;
  mat.color.r = 1.0;
  mat.color.g = 1.0;
  mat.color.b = 1.0;
  material_->setMaterial(mat);

  inst_->setMaterial(material_);
}

SimpleShape::~SimpleShape()
{
  delete inst_;
  material_.reset();
}

void SimpleShape::setColor(const Ogre::ColourValue& col)
{
  rviz_msgs::Material mat;
  mat.has_color = true;
  mat.opacity = col.a;
  mat.color.r = col.r;
  mat.color.g = col.g;
  mat.color.b = col.b;
  material_->setMaterial(mat);
}

} // namespace rviz_renderer_ogre