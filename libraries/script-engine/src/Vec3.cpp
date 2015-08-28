//
//  Vec3.cpp
//  libraries/script-engine/src
//
//  Created by Brad Hefta-Gaub on 1/29/14.
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include <glm/gtx/vector_angle.hpp>

#include <QDebug>

#include "ScriptEngineLogging.h"
#include "NumericalConstants.h"
#include "Vec3.h"

glm::vec3 Vec3::reflect(const glm::vec3& v1, const glm::vec3& v2) {
    return glm::reflect(v1, v2);
}
glm::vec3 Vec3::cross(const glm::vec3& v1, const glm::vec3& v2) {
    return glm::cross(v1,v2);
}

float Vec3::dot(const glm::vec3& v1, const glm::vec3& v2) {
    return glm::dot(v1,v2);
}

glm::vec3 Vec3::multiply(const glm::vec3& v1, float f) {
    return v1 * f;
}

glm::vec3 Vec3::multiply(float f, const glm::vec3& v1) {
    return v1 * f;
}

glm::vec3 Vec3::multiplyQbyV(const glm::quat& q, const glm::vec3& v) {
    return q * v;
}

glm::vec3 Vec3::sum(const glm::vec3& v1, const glm::vec3& v2) { 
    return v1 + v2;
}
glm::vec3 Vec3::subtract(const glm::vec3& v1, const glm::vec3& v2) {
    return v1 - v2;
}

float Vec3::length(const glm::vec3& v) {
    return glm::length(v);
}

float Vec3::distance(const glm::vec3& v1, const glm::vec3& v2) {
    return glm::distance(v1, v2);
}

float Vec3::orientedAngle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
    return glm::degrees(glm::orientedAngle(glm::normalize(v1), glm::normalize(v2), glm::normalize(v3)));
}

glm::vec3 Vec3::normalize(const glm::vec3& v) {
    return glm::normalize(v);
}

glm::vec3 Vec3::mix(const glm::vec3& v1, const glm::vec3& v2, float m) {
    return glm::mix(v1, v2, m);
}

void Vec3::print(const QString& lable, const glm::vec3& v) {
    qCDebug(scriptengine) << qPrintable(lable) << v.x << "," << v.y << "," << v.z;
}

bool Vec3::equal(const glm::vec3& v1, const glm::vec3& v2) {
    return v1 == v2;
}

glm::vec3 Vec3::toPolar(const glm::vec3& v) {
    float radius = length(v);
    if (glm::abs(radius) < EPSILON) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
    
    glm::vec3 u = v / radius;
    
    float elevation, azimuth;
    
    elevation = glm::asin(-u.y);
    azimuth = atan2(v.x, v.z);
    
    // Round off small decimal values
    if (glm::abs(elevation) < EPSILON) {
        elevation = 0.0f;
    }
    if (glm::abs(azimuth) < EPSILON) {
        azimuth = 0.0f;
    }

    return glm::vec3(elevation, azimuth, radius);
}

glm::vec3 Vec3::fromPolar(const glm::vec3& polar) {
    float x = glm::cos(polar.x) * glm::sin(polar.y);
    float y = glm::sin(-polar.x);
    float z = glm::cos(polar.x) * glm::cos(polar.y);
    
    // Round small values to 0
    if (glm::abs(x) < EPSILON) {
        x = 0.0f;
    }
    if (glm::abs(y) < EPSILON) {
        y = 0.0f;
    }
    if (glm::abs(z) < EPSILON) {
        z = 0.0f;
    }
   
    return polar.z * glm::vec3(x, y, z);
}

glm::vec3 Vec3::fromPolar(float elevation, float azimuth) {
    glm::vec3 v = glm::vec3(elevation, azimuth, 1.0f);
    return fromPolar(v);
}

const vec3& Vec3::UNIT_X() {
    return Vectors::UNIT_X;
}

const vec3& Vec3::UNIT_Y() {
    return Vectors::UNIT_Y;
}

const vec3& Vec3::UNIT_Z() {
    return Vectors::UNIT_Z;
}

const vec3& Vec3::UNIT_NEG_X() {
    return Vectors::UNIT_NEG_X;
}

const vec3& Vec3::UNIT_NEG_Y() {
    return Vectors::UNIT_NEG_Y;
}

const vec3& Vec3::UNIT_NEG_Z() {
    return Vectors::UNIT_NEG_Z;
}

const vec3& Vec3::UNIT_XY() {
    return Vectors::UNIT_XY;
}

const vec3& Vec3::UNIT_XZ() {
    return Vectors::UNIT_XZ;
}

const vec3& Vec3::UNIT_YZ() {
    return Vectors::UNIT_YZ;
}

const vec3& Vec3::UNIT_XYZ() {
    return Vectors::UNIT_XYZ;
}

const vec3& Vec3::FLOAT_MAX() {
    return Vectors::MAX;
}

const vec3& Vec3::FLOAT_MIN() {
    return Vectors::MIN;
}

const vec3& Vec3::ZERO() {
    return Vectors::ZERO;
}

const vec3& Vec3::ONE() {
    return Vectors::ONE;
}

const vec3& Vec3::RIGHT() {
    return Vectors::RIGHT;
}

const vec3& Vec3::UP() {
    return Vectors::UNIT_X;
}

const vec3& Vec3::FRONT() {
    return Vectors::FRONT;
}
