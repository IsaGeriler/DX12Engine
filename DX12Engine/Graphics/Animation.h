#pragma once

#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "../Core/MyMath.h"

struct Bone {
	std::string name;
	Matrix offset;
	int parentIndex;
};

struct Skeleton {
	std::vector<Bone> bones;
	Matrix globalInverse;
};

struct AnimationFrame {
	std::vector<Vec3> positions;
	std::vector<Quaternion> rotations;
	std::vector<Vec3> scales;
};

class AnimationSequence {
public:
	std::vector<AnimationFrame> frames;
	float ticksPerSeconds;

	Vec3 interpolate(Vec3 p1, Vec3 p2, float t) {
		return ((p1 * (1.f - t)) + (p2 * t));
	}

	Quaternion interpolate(Quaternion q1, Quaternion q2, float t) {
		return Quaternion::slerp(q1, q2, t);
	}

	float duration() {
		return ((float)frames.size() / ticksPerSeconds);
	}

	void calculateFrame(float t, int& frame, float& interpolationFact) {
		interpolationFact = t * ticksPerSeconds;
		frame = (int)floorf(interpolationFact);
		interpolationFact = interpolationFact - (float)frame;
		frame = std::min(frame, (int)(frames.size() - 1));
	}

	int nextFrame(int frame) {
		return std::min(frame + 1, (int)(frames.size() - 1));
	}

	Matrix interpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex) {
		Matrix scale = Matrix::scale(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrame(baseFrame)].scales[boneIndex], interpolationFact));
		Matrix rotation = interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrame(baseFrame)].rotations[boneIndex], interpolationFact).toMatrix();
		Matrix translate = Matrix::translate(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrame(baseFrame)].positions[boneIndex], interpolationFact));

		// Matrix local = translate * rotation * scale;  // Column-Major
		Matrix local = scale * rotation * translate;     // Row-Major

		if (skeleton->bones[boneIndex].parentIndex > -1) {
			// Matrix global = matrices[skeleton->bones[boneIndex].parentIndex] * local;  // Column-Major
			Matrix global = local * matrices[skeleton->bones[boneIndex].parentIndex];     // Row-Major
			return global;
		}
		return local;
	}
};

class Animation {
public:
	std::map<std::string, AnimationSequence> animations;
	Skeleton skeleton;
	
	void calculateFrame(std::string name, float t, int& frame, float& interpolationFact) {
		animations[name].calculateFrame(t, frame, interpolationFact);
	}

	Matrix interpolateBoneToGlobal(std::string name, Matrix* matrices, int baseFrame, float interpolationFact, int boneIndex) {
		return animations[name].interpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
	}

	void calculateFinalTransforms(Matrix* matrices) {
		for (int i = 0; i < skeleton.bones.size(); i++) {
			// matrices[i] = matrices[i] * skeleton.bones[i].offset * skeleton.globalInverse;  // Column-Major
			matrices[i] = skeleton.bones[i].offset * matrices[i] * skeleton.globalInverse;     // Row-Major
		}
	}
};

class AnimationInstance {
public:
	Animation* animation;
	std::string current_animation;
	float t;
	Matrix matrices[256];

	void initialize(Animation* _animation) {
		animation = _animation;
	}

	void resetAnimationTime() {
		t = 0.f;
	}

	bool isAnimationFinished() {
		return (t > animation->animations[current_animation].duration());
	}

	void updateAnimation(std::string name, float dt) {
		if (name == current_animation) { t += dt; }
		else { current_animation = name; t = 0; }
		if (isAnimationFinished()) resetAnimationTime();

		int frame = 0;
		float interpolationFact = 0.f;
		animation->calculateFrame(name, t, frame, interpolationFact);

		for (int i = 0; i < animation->skeleton.bones.size(); i++) {
			matrices[i] = animation->interpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
		}
		animation->calculateFinalTransforms(matrices);
	}
};