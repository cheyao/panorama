#pragma once

#include "components/component.hpp"
#include "third_party/Eigen/Geometry"

class CameraComponent : public Component {
  public:
	explicit CameraComponent(class Actor* owner, int priority = 200);
	CameraComponent(CameraComponent&&) = delete;
	CameraComponent(const CameraComponent&) = delete;
	CameraComponent& operator=(CameraComponent&&) = delete;
	CameraComponent& operator=(const CameraComponent&) = delete;
	~CameraComponent() override = default;

	void update(float delta) override;

	void setFOV(float fov) { mFOV = fov; }

	void project();
	void view();

	class Actor* getOwner() { return mOwner; }

	Eigen::Affine3f getViewMatrix() { return mViewMatrix; };
	Eigen::Affine3f getProjectionMatrix() { return mProjectionMatrix; };

  private:
	float mFOV;

	Eigen::Affine3f mViewMatrix;
	Eigen::Affine3f mProjectionMatrix;
};
