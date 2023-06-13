#ifndef __PARTICLE_H__
#define __PARTICLE_H__
#include "Point.h"

// SDL has a SDL_Color, but because of linking problems I create our own
struct Color {
	void Set(int red, int green, int blue, int alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};


class Particle {
	// methods
public:
	Particle();
	~Particle();

	void Initialize(fPoint initialPosition, fPoint initiaVelocity, fPoint acceleration, Color initialColor, Color objectiveColor, float initialScale, float objectiveScale, float lifespan);

	void Update(float dt);

	void ResetParticle();

	bool IsBeingUsed();

	// get the next free particle in the pool
	Particle* GetNext() const {
		return next;
	}

	void SetNext(Particle* next) {
		this->next = next;
	}

	fPoint GetPosition() const {
		return position;
	}

	Color GetColor() const {
		return color;
	}

	float GetScale() const {
		return scale;
	}

protected:
private:

	// variables
public:
	

protected:
private:

	float timeLeft = 0;

	fPoint position;
	fPoint velocity;
	fPoint acceleration;

	Color initialColor;
	Color color;
	Color objectiveColor;

	float initialScale = 0;
	float scale = 0;
	float objectiveScale = 0;

	float lifespan = 0;

	Particle* next;
};

#endif