#pragma once
#ifndef POSE_HELPER_H
#define POSE_HELPER_H

typedef ml::vec6f Pose;

namespace PoseHelper {

	//! assumes z-y-x rotation composition (euler angles)
	static Pose MatrixToPose(const ml::mat4f& Rt) {
		ml::mat3f R = Rt.getRotation();
		ml::vec3f tr = Rt.getTranslation();

		float eps = 0.00001f;

		float psi, theta, phi; // x,y,z axis angles
		if (abs(R(2, 0) - 1) > eps || abs(R(2, 0) + 1) > eps) { // R(2, 0) != +/- 1
			theta = -asin(R(2, 0)); // \pi - theta
			float costheta = cos(theta);
			psi = atan2(R(2, 1) / costheta, R(2, 2) / costheta);
			phi = atan2(R(1, 0) / costheta, R(0, 0) / costheta);
		}
		else {
			phi = 0;
			if (abs(R(2, 0) + 1) > eps) {
				theta = ml::math::PIf / 2.0f;
				psi = phi + atan2(R(0, 1), R(0, 2));
			}
			else {
				theta = -ml::math::PIf / 2.0f;
				psi = -phi + atan2(-R(0, 1), -R(0, 2));
			}
		}

		return Pose(psi, theta, phi, tr.x, tr.y, tr.z);
	}

	//! assumes z-y-x rotation composition (euler angles)
	static ml::mat4f PoseToMatrix(const Pose& ksi) {
		ml::mat4f res; res.setIdentity();
		ml::vec3f degrees;
		for (unsigned int i = 0; i < 3; i++)
			degrees[i] = ml::math::radiansToDegrees(ksi[i]);
		res.setRotation(ml::mat3f::rotationZ(degrees[2])*ml::mat3f::rotationY(degrees[1])*ml::mat3f::rotationX(degrees[0]));
		res.setTranslationVector(ml::vec3f(ksi[3], ksi[4], ksi[5]));
		return res;
	}

	static std::vector<ml::mat4f> convertToMatrices(const std::vector<Pose>& poses) {
		std::vector<ml::mat4f> matrices(poses.size());

		for (unsigned int i = 0; i < poses.size(); i++)
			matrices[i] = PoseHelper::PoseToMatrix(poses[i]);

		return matrices;
	}
	static std::vector<Pose> convertToPoses(const std::vector<ml::mat4f>& matrices) {
		std::vector<Pose> poses(matrices.size());

		for (unsigned int i = 0; i < matrices.size(); i++)
			poses[i] = PoseHelper::MatrixToPose(matrices[i]);

		return poses;
	}
}

#endif