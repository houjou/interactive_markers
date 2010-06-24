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

#ifndef RVIZ_RENDERER_OGRE_SIMPLE_SHAPE_H
#define RVIZ_RENDERER_OGRE_SIMPLE_SHAPE_H

#include <boost/shared_ptr.hpp>

namespace Ogre
{
class SceneManager;
class SceneNode;
class ColourValue;
}

namespace rviz_math
{
class Vector3;
class Quaternion;
}

namespace rviz_renderer_ogre
{

class TransformNode;
class MeshInstance;
class SimpleColorMaterial;
typedef boost::shared_ptr<SimpleColorMaterial> SimpleColorMaterialPtr;

class SimpleShape
{
public:
  enum Type
  {
    Cone,
    Cube,
    Cylinder,
    Sphere
  };

  SimpleShape(Ogre::SceneManager* scene_manager, Type type, TransformNode* node);
  ~SimpleShape();

  void setColor(const Ogre::ColourValue& col);

private:
  Ogre::SceneManager* scene_manager_;
  MeshInstance* inst_;

  SimpleColorMaterialPtr material_;
};

} // namespace rviz_renderer_ogre

#endif // RVIZ_RENDERER_OGRE_SIMPLE_SHAPE_H