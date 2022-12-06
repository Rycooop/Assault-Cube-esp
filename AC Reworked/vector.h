#ifndef AC_VECTOR_H
#define AC_VECTOR_H


struct Vector2 {
	Vector2() {
		this->x = 0.0;
		this->y = 0.0;
	}

	Vector2(int x, int y) {
		this->x = x;
		this->y = y;
	}

	Vector2 operator+(const Vector2& toAdd) {
		return Vector2(this->x + toAdd.x, this->y + toAdd.y);
	}

	Vector2 operator-(const Vector2& toSub) {
		return Vector2(this->x - toSub.x, this->y - toSub.y);
	}

	void operator+=(const Vector2& toAdd) {
		this->x += toAdd.x;
		this->y += toAdd.y;
	}

	void operator-=(const Vector2& toSub) {
		this->x -= toSub.x;
		this->y -= toSub.y;
	}

	void operator=(const Vector2& equals) {
		this->x = equals.x;
		this->y = equals.y;
	}

	float x, y;
};

struct Vector3 {
	Vector3() {
		this->x = 0.0;
		this->y = 0.0;
		this->z = 0.0;
	}

	Vector3(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3 operator+(const Vector3& toAdd) {
		return Vector3(this->x + toAdd.x, this->y + toAdd.y, this->z + toAdd.z);
	}

	Vector3 operator-(const Vector3& toSub) {
		return Vector3(this->x - toSub.x, this->y - toSub.y, this->z - toSub.z);
	}

	void operator+=(const Vector3& toAdd) {
		this->x += toAdd.x;
		this->y += toAdd.y;
		this->z = toAdd.z;
	}

	void operator-=(const Vector3& toSub) {
		this->x -= toSub.x;
		this->y -= toSub.y;
		this->z -= toSub.z;
	}

	void operator=(const Vector3& equals) {
		this->x = equals.x;
		this->y = equals.y;
		this->z = equals.z;
	}

	float x, y, z;
};

struct Vector4 {
	Vector4() {
		this->x = 0.0;
		this->y = 0.0;
		this->z = 0.0;
		this->w = 0.0;
	}

	Vector4(int x, int y, int z, int w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	float x, y, z, w;
};

#endif //AC_VECTOR_H