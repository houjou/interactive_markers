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

#include <rviz_renderer_client/simple_shape.h>
#include <rviz_renderer_client/init.h>

#include <rviz_math/vector3.h>
#include <rviz_math/quaternion.h>

#include <rviz_interfaces/SimpleShape.h>

using namespace rviz_math;

namespace rviz_renderer_client
{

SimpleShape::SimpleShape()
: proxy_(0)
{}

SimpleShape::SimpleShape(const rviz_uuid::UUID& scene_id, const rviz_uuid::UUID& id)
: Object(id)
, scene_id_(scene_id)
{
  proxy_ = getProxyInterface<rviz_interfaces::SimpleShapeProxy>("simple_shape");
}

void SimpleShape::setPosition(const rviz_math::Vector3& pos)
{
  proxy_->setPosition(scene_id_, getID(), pos);
}

void SimpleShape::setPosition(float x, float y, float z)
{
  proxy_->setPosition(scene_id_, getID(), Vector3(x, y, z));
}

void SimpleShape::setOrientation(const rviz_math::Quaternion& orient)
{
  proxy_->setOrientation(scene_id_, getID(), orient);
}

void SimpleShape::setOrientation(float x, float y, float z, float w)
{
  proxy_->setOrientation(scene_id_, getID(), Quaternion(x, y, z, w));
}

void SimpleShape::setScale(const rviz_math::Vector3& scale)
{
  proxy_->setScale(scene_id_, getID(), scale);
}

void SimpleShape::setScale(float x, float y, float z)
{
  proxy_->setScale(scene_id_, getID(), Vector3(x, y, z));
}


}
