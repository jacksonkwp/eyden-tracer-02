#pragma once

#include "ShaderFlat.h"

/**
 * @brief Eye-light shader class
 */
class CShaderEyelight : public CShaderFlat
{
public:
	/**
	 * @brief Constructor
	 * @details This is a texture-free and light-source-free shader
	 * @param scene Reference to the scene
	 * @param color The color of the object
	 */
	CShaderEyelight(Vec3f color = RGB(0.5f, 0.5f, 0.5f))
		: CShaderFlat(color)
	{}
	virtual ~CShaderEyelight(void) = default;

	virtual Vec3f shade(const Ray& ray) const override
	{
		Vec3f norm = ray.hit->getNormal(ray);
		double cosTheta = norm.dot(ray.dir) / (sqrt(norm.dot(norm)) * sqrt(ray.dir.dot(ray.dir)));
		return abs(cosTheta) * m_color;
	}
};

