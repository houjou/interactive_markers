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

#include <rviz_renderer_server/server.h>
#include <rviz_renderer_interface/irenderer.h>
#include <rviz_renderer_interface/irender_window.h>
#include <rviz_renderer_interface/iscene.h>
#include <rviz_renderer_interface/icamera.h>
#include <rviz_renderer_interface/irender_loop_listener.h>
#include <rviz_uuid/uuid.h>
#include <rviz_math/vector3.h>
#include <rviz_math/quaternion.h>

#include <ros/ros.h>
#include <ros/callback_queue.h>

#include <rviz_interfaces/Camera.h>
#include <rviz_interfaces/RenderWindow.h>
#include <rviz_interfaces/Scene.h>
#include <rviz_interfaces/SimpleShape.h>

using namespace rviz_uuid;

namespace rviz_renderer_server
{

class Server::RenderLoopListener : public rviz_renderer_interface::IRenderLoopListener
{
public:
  RenderLoopListener(ros::CallbackQueue* queue)
  : queue_(queue)
  {

  }

  virtual void postRender(rviz_renderer_interface::IRenderer* renderer)
  {
    queue_->callAvailable();
  }

private:
  ros::CallbackQueue* queue_;
};

class CameraServer : public rviz_interfaces::CameraServer
{
public:
  CameraServer(rviz_renderer_interface::IRenderer* rend, const std::string& name, const ros::NodeHandle& nh)
  : rviz_interfaces::CameraServer(name, nh)
  , renderer_(rend)
  {
  }

  rviz_renderer_interface::ICamera* lookupCamera(const rviz_uuid::UUID& id)
  {
    rviz_renderer_interface::ICamera* cam = renderer_->getCamera(id);
    if (!cam)
    {
      std::stringstream ss;
      ss << "Invalid camera [" << id << "]";
      throw std::runtime_error(ss.str());
    }

    return cam;
  }

  virtual void setPosition(const rviz_msgs::UUID& id, const geometry_msgs::Vector3& pos)
  {
    lookupCamera(id)->setPosition(pos);
  }

  virtual void setOrientation(const rviz_msgs::UUID& id, const geometry_msgs::Quaternion& orient)
  {
    lookupCamera(id)->setOrientation(orient);
  }

  virtual void lookAt(const rviz_msgs::UUID& id, const geometry_msgs::Vector3& pos)
  {
    lookupCamera(id)->lookAt(pos);
  }

  virtual void move(const rviz_msgs::UUID& id, const geometry_msgs::Vector3& vec)
  {
    lookupCamera(id)->move(vec);
  }

  virtual void moveRelative(const rviz_msgs::UUID& id, const geometry_msgs::Vector3& vec)
  {
    lookupCamera(id)->moveRelative(vec);
  }

  virtual void rotate(const rviz_msgs::UUID& id, const geometry_msgs::Quaternion& quat)
  {
    lookupCamera(id)->rotate(quat);
  }

  virtual void setFOVY(const rviz_msgs::UUID& id, float fovy)
  {
    lookupCamera(id)->setFOVY(fovy);
  }

  virtual void setAspectRatio(const rviz_msgs::UUID& id, float aspect)
  {
    lookupCamera(id)->setAspectRatio(aspect);
  }

  virtual void setAutoAspectRatio(const rviz_msgs::UUID& id, uint8_t autoaspect)
  {
    lookupCamera(id)->setAutoAspectRatio(autoaspect);
  }

  virtual void setNearClipDistance(const rviz_msgs::UUID& id, float dist)
  {
    lookupCamera(id)->setNearClipDistance(dist);
  }

  virtual void setFarClipDistance(const rviz_msgs::UUID& id, float dist)
  {
    lookupCamera(id)->setFarClipDistance(dist);
  }

private:
  rviz_renderer_interface::IRenderer* renderer_;
};

class RenderWindowServer : public rviz_interfaces::RenderWindowServer
{
public:
  RenderWindowServer(rviz_renderer_interface::IRenderer* rend, const std::string& name, const ros::NodeHandle& nh)
  : rviz_interfaces::RenderWindowServer(name, nh)
  , renderer_(rend)
  {
  }

  virtual void resized(const rviz_msgs::UUID& id, uint32_t width, uint32_t height)
  {
    rviz_renderer_interface::IRenderWindow* wnd = renderer_->getRenderWindow(id);
    wnd->resized(width, height);
  }

  virtual void attachCamera(const rviz_msgs::UUID& id, const rviz_msgs::UUID& camera_id)
  {
    rviz_renderer_interface::IRenderWindow* wnd = renderer_->getRenderWindow(id);
    wnd->attachCamera(camera_id);
  }

  virtual void create(const rviz_msgs::UUID& id, const std::string& parent_window, uint32_t width, uint32_t height)
  {
    renderer_->createRenderWindow(id, parent_window, width, height);
  }

  virtual void destroy(const rviz_msgs::UUID& id)
  {
    renderer_->destroyRenderWindow(id);
  }

private:
  rviz_renderer_interface::IRenderer* renderer_;
};

class SceneServer : public rviz_interfaces::SceneServer
{
public:
  SceneServer(rviz_renderer_interface::IRenderer* rend, const std::string& name, const ros::NodeHandle& nh)
  : rviz_interfaces::SceneServer(name, nh)
  , renderer_(rend)
  {
  }

  virtual void create(const rviz_msgs::UUID& id)
  {
    rviz_renderer_interface::IScene* scene = renderer_->createScene(id);
    if (!scene)
    {
      throw std::runtime_error("Could not create scene [" + UUID(id).toString() + "]");
    }
  }

  virtual void destroy(const rviz_msgs::UUID& id)
  {
    renderer_->destroyScene(id);
  }

  virtual void createCamera(const rviz_msgs::UUID& id, const rviz_msgs::UUID& camera_id)
  {
    rviz_renderer_interface::IScene* scene = renderer_->getScene(id);
    if (!scene)
    {
      throw std::runtime_error("Scene [" + UUID(id).toString() + "] does not exist");
    }

    scene->createCamera(camera_id);
  }

  virtual void destroyCamera(const rviz_msgs::UUID& id, const rviz_msgs::UUID& camera_id)
  {
    rviz_renderer_interface::IScene* scene = renderer_->getScene(id);
    if (!scene)
    {
      throw std::runtime_error("Scene [" + UUID(id).toString() + "] does not exist");
    }

    scene->destroyCamera(camera_id);
  }

  virtual void createSimpleShape(const rviz_msgs::UUID& scene_id, const rviz_msgs::UUID& shape_id, const std::string& type)
  {
    rviz_renderer_interface::ISimpleShape::Type t;
    if (type == "cone")
    {
      t = rviz_renderer_interface::ISimpleShape::Cone;
    }
    else if (type == "cube")
    {
      t = rviz_renderer_interface::ISimpleShape::Cube;
    }
    else if (type == "cylinder")
    {
      t = rviz_renderer_interface::ISimpleShape::Cylinder;
    }
    else if (type == "sphere")
    {
      t = rviz_renderer_interface::ISimpleShape::Sphere;
    }
    else
    {
      throw std::runtime_error("Unrecognized simple shape [" + type + "]");
    }

    rviz_renderer_interface::IScene* scene = renderer_->getScene(scene_id);
    if (!scene)
    {
      throw std::runtime_error("Scene [" + UUID(scene_id).toString() + "] does not exist");
    }

    scene->createSimpleShape(shape_id, t);
  }

  virtual void destroySimpleShape(const rviz_msgs::UUID& scene_id, const rviz_msgs::UUID& shape_id)
  {
    rviz_renderer_interface::IScene* scene = renderer_->getScene(scene_id);
    if (!scene)
    {
      throw std::runtime_error("Scene [" + UUID(scene_id).toString() + "] does not exist");
    }

    scene->destroySimpleShape(shape_id);
  }

private:
  rviz_renderer_interface::IRenderer* renderer_;
};

class SimpleShapeServer : public rviz_interfaces::SimpleShapeServer
{
public:
  SimpleShapeServer(rviz_renderer_interface::IRenderer* rend, const std::string& name, const ros::NodeHandle& nh)
  : rviz_interfaces::SimpleShapeServer(name, nh)
  , renderer_(rend)
  {
  }

  virtual void setPosition(const rviz_msgs::UUID& scene_id, const rviz_msgs::UUID& id, const geometry_msgs::Vector3& pos)
  {
    rviz_renderer_interface::IScene* scene = renderer_->getScene(scene_id);
    rviz_renderer_interface::ISimpleShape* shape = scene->getSimpleShape(id);
    shape->setPosition(pos);
  }

  virtual void setOrientation(const rviz_msgs::UUID& scene_id, const rviz_msgs::UUID& id, const geometry_msgs::Quaternion& orient)
  {
    rviz_renderer_interface::IScene* scene = renderer_->getScene(scene_id);
    rviz_renderer_interface::ISimpleShape* shape = scene->getSimpleShape(id);
    shape->setOrientation(orient);
  }

  virtual void setScale(const rviz_msgs::UUID& scene_id, const rviz_msgs::UUID& id, const geometry_msgs::Vector3& scale)
  {
    rviz_renderer_interface::IScene* scene = renderer_->getScene(scene_id);
    rviz_renderer_interface::ISimpleShape* shape = scene->getSimpleShape(id);
    shape->setScale(scale);
  }

private:
  rviz_renderer_interface::IRenderer* renderer_;
};

Server::Server(rviz_renderer_interface::IRenderer* renderer, const ros::NodeHandle& nh)
: renderer_(renderer)
, callback_queue_(new ros::CallbackQueue)
, nh_(new ros::NodeHandle(nh, "renderer"))
, render_loop_listener_(new RenderLoopListener(callback_queue_.get()))
{
  nh_->setCallbackQueue(callback_queue_.get());
  camera_server_.reset(new CameraServer(renderer_, "camera", *nh_));
  render_window_server_.reset(new RenderWindowServer(renderer_, "render_window", *nh_));
  scene_server_.reset(new SceneServer(renderer_, "scene", *nh_));
  simple_shape_server_.reset(new SimpleShapeServer(renderer_, "simple_shape", *nh_));

  renderer_->addRenderLoopListener(render_loop_listener_.get());
}

Server::~Server()
{
  renderer_->removeRenderLoopListener(render_loop_listener_.get());
}


} // namespace rviz_renderer_server
