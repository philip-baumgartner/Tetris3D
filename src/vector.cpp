#include "vector.h"
#include <assert.h>
#include <math.h>

Vector::Vector(float x, float y, float z) {
	this->X = x;
	this->Y = y;
	this->Z = z;
}

Vector::Vector() {
	this->X = 0;
	this->Y = 0;
	this->Z = 0;
}

// v = v1 + v2
Vector Vector::operator +(const Vector &v) const {
	Vector result(0, 0, 0);
	result.X = this->X + v.X;
	result.Y = this->Y + v.Y;
	result.Z = this->Z + v.Z;
	return result;
}

// v = v1 - v2
Vector Vector::operator -(const Vector &v) const {
	Vector result(0, 0, 0);
	result.X = this->X - v.X;
	result.Y = this->Y - v.Y;
	result.Z = this->Z - v.Z;
	return result;
}

// v = -v
Vector Vector::operator-() const {
	Vector result(0, 0, 0);
	result.X = -(this->X);
	result.Y = -(this->Y);
	result.Z = -(this->Z);
	return result;
}

// v = v + v1
Vector &Vector::operator +=(const Vector &v) {
	this->X += v.X;
	this->Y += v.Y;
	this->Z += v.Z;
	return *this;
}

// v = v * c
Vector Vector::operator *(float c) const {
	Vector result(0, 0, 0);
	result.X = this->X * c;
	result.Y = this->Y * c;
	result.Z = this->Z * c;
	return result;
}

// f = v ° v1, Lagebeziehung zweier Vektoren
float Vector::dot(const Vector &v) const {
	return this->X * v.X + this->Y * v.Y + this->Z * v.Z;
}

// v = v x v1, Erzeugt Vektor, der senkrecht zu beiden Vektoren steht
Vector Vector::cross(const Vector &v) const {
	Vector result(0, 0, 0);
	result.X = this->Y * v.Z - this->Z * v.Y;
	result.Y = -(this->X * v.Z - this->Z * v.X);
	result.Z = this->X * v.Y - this->Y * v.X;
	return result;
}

// v = 1 / |v| * v, Länge des Vektors = 1
Vector &Vector::normalize() {
	Vector n(0, 0, 0);
	if (this->length() == 0) { // Exception?
		return n;
	}
	float factor = 1 / this->length();
	n = *(this) * factor;

	this->X = n.X;
	this->Y = n.Y;
	this->Z = n.Z;

	return *this;
}

// v = sqrt(vx² + vy² + vz²)
float Vector::length() const {
	return sqrtf(this->lengthSquared());
}

// v = sqrt(vx² + vy² + vz²)² = vx² + vy² + vz²
float Vector::lengthSquared() const {
	return pow(this->X, 2) + pow(this->Y, 2) + pow(this->Z, 2);
}

// v = v - 2 * (v ° vn) * n, Reflexion an Normale, Eintrittswinkel = Austrittswinkel
Vector Vector::reflection(const Vector &normal) const {
	float dotProduct = this->dot(normal);
	Vector result(0, 0, 0);
	result.X = this->X - 2 * dotProduct * normal.X;
	result.Y = this->Y - 2 * dotProduct * normal.Y;
	result.Z = this->Z - 2 * dotProduct * normal.Z;
	return result;
}

bool Vector::triangleIntersection(const Vector &d, const Vector &a, const Vector &b, const Vector &c, float &s) const {
	Vector p(0, 0, 0);
	Vector n(0, 0, 0);
	float df = 0;

	// Normalenvektor der Ebene bestimmen, a ist der Aufpunkt der Ebene
	// Die Ebene kann dann dargestellt werden durch d = n*a (d -> Abstand Ursprung - Ebene auf n)
	n += (b - a).cross((c - a));
	n.normalize();

	// Skalarprodukt des Richtungsvektors der Gerade und des Normalenvektors der Ebene.
	// Bei 0 -> Strahl liegt in der Ebene (!= Schnittpunkt)/ Strahl ist parallel zur Ebene.
	if (n.dot(d) == 0)
		return false;

	// Projektion auf den Normalenvektor n der Ebene ergibt Abstand Koord.-Ursprung - n.
	// Gleichsetzen Ebene - Strahl ergibt s (Faktor "Zeit" wann Strahl Ebene erreicht)
	df = n.dot(a);
	s = (df - n.dot(*this)) / n.dot(d);

	if (s <= 0)
		return false;

	// Schnittpunkt Strahl-Ebene ermitteln, Faktor s in Strahlgleichung einsetzen
	p = (*this) + d * s;

	// Prüfen, ob Schnittpunkt innerhalb des Dreieckes liegt.
	// Fläche des Dreiecks muss größer sein als die Flächen der drei einzelnen Dreiecke
	float Aabc = ((b - a).cross((c - a))).length() / 2;
	float Aabp = ((b - a).cross((p - a))).length() / 2;
	float Aacp = ((c - a).cross((p - a))).length() / 2;
	float Abcp = ((c - b).cross((p - b))).length() / 2;

	return Aabc + EPSILON >= (Aabp + Aacp + Abcp);
}

Vector Vector::calcNormal(const Vector &a, const Vector &b, const Vector &c) {
	Vector ba = b - a;
	Vector ca = c - a;
	Vector result = ba.cross(ca);

	return result.normalize();
}