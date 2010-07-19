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

#ifndef RVIZ_ROS_CLIENT_CAMERA_H
#define RVIZ_ROS_CLIENT_CAMERA_H

#include "object.h"

namespace rviz_interfaces
{
class CameraProxy;
}

namespace rviz_math
{
class Vector3;
class Quaternion;
}

namespace rviz_renderer_client
{

class Camera : public Object
{
public:
  Camera();
  Camera(const rviz_uuid::UUID& id, const rviz_uuid::UUID& scene_id);

  const rviz_uuid::UUID& getSceneID() { return scene_id_; }

  void setPosition(const rviz_math::Vector3& pos);
  void setOrientation(const rviz_math::Quaternion& orient);
  void lookAt(const rviz_math::Vector3& point);
  void move(const rviz_math::Vector3& v);
  void moveRelative(const rviz_math::Vector3& v);
  void rotate(const rviz_math::Quaternion& q);
  void rotateRelative(const rviz_math::Quaternion& q);
  void setFOVY(float fovy);
  void setAspectRatio(float aspect);
  void setAutoAspectRatio(bool autoaspect);
  void setNearClipDistance(float near_clip);
  void setFarClipDistance(float far_clip);

  void setTransform(const rviz_math::Vector3& pos, const rviz_math::Quaternion& orient);

private:
  rviz_uuid::UUID scene_id_;
  rviz_interfaces::CameraProxy* proxy_;
};

} // namespace rviz_renderer_client

#endif // RVIZ_ROS_CLIENT_CAMERA_H