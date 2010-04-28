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

#include "rviz_renderer_client/scene.h"
#include "rviz_renderer_client/camera.h"
#include "rviz_renderer_client/init.h"
#include "rviz_renderer_client/simple_shape.h"

#include <rviz_interfaces/Scene.h>

using namespace rviz_uuid;
using namespace rviz_renderer_client;

Scene rviz_renderer_client::createScene()
{
  rviz_interfaces::SceneProxy* proxy = getProxyInterface<rviz_interfaces::SceneProxy>("scene");
  UUID id = UUID::Generate();
  proxy->create(id);
  return Scene(id);
}

void rviz_renderer_client::destroyScene(const Scene& scene)
{
  rviz_interfaces::SceneProxy* proxy = getProxyInterface<rviz_interfaces::SceneProxy>("scene");
  proxy->destroy(scene.getID());
}

Scene::Scene()
: proxy_(0)
{
}

Scene::Scene(const UUID& id)
: Object(id)
{
  proxy_ = getProxyInterface<rviz_interfaces::SceneProxy>("scene");
}

Camera Scene::createCamera()
{
  UUID id = UUID::Generate();
  proxy_->createCamera(getID(), id);

  return Camera(id, getID());
}

void Scene::destroyCamera(const Camera& cam)
{
  proxy_->destroyCamera(getID(), cam.getID());
}

SimpleShape Scene::createSimpleShape(const std::string& type)
{
  UUID id = UUID::Generate();
  proxy_->createSimpleShape(getID(), id, type);
  return SimpleShape(getID(), id);
}

void Scene::destroySimpleShape(const SimpleShape& shape)
{
  proxy_->destroySimpleShape(getID(), shape.getID());
}
