// Scene class
// Written by Sergey Kosov in 2019 for Jacobs University
#pragma once

#include "ILight.h"
#include "IPrim.h"
#include "ICamera.h"

// ================================ Scene Class ================================
/**
 * @brief Scene class
 */
class CScene
{
public:
	/**
	 * @brief Constructor
	 * @param camera The reference to the camera
	 * @todo Background may be image
	 */
	CScene(Vec3f bgColor = RGB(0,0,0))
		: m_bgColor(bgColor)
	{}
  	~CScene(void) = default;
  
	/**
	 * @brief Adds a new primitive to the scene
	 * @param prim Pointer to the primitive
	 */
	void add(const ptr_prim_t pPrim)
	{
		m_vpPrims.push_back(pPrim);
	}
	/**
	 * @brief Adds a new light to the scene
	 * @param pLight Pointer to the light
	 */
	void add(const ptr_light_t pLight)
	{
		m_vpLights.push_back(pLight);
	}
	/**
	 * @brief Adds a new camera to the scene and makes it to ba active
	 * @param pCamera Pointer to the camera
	 */
	void add(const ptr_camera_t pCamera)
	{
		m_vpCameras.push_back(pCamera);
	}
	/**
	 * @brief Returns the container with all scene light source objects
	 * @note This method is to be used only in OpenRT shaders
	 * @return The vector with pointers to the scene light sources
	 */
	const std::vector<ptr_light_t>	getLights(void) const { return m_vpLights; }
	/**
	 * @brief Returns the active camera
	 * @retval ptr_camera_t The pointer to active camera
	 * @retval nullptr If there are no cameras added yet into the scene
	 */
	ptr_camera_t getActiveCamera(void) const { return m_vpCameras.empty() ? nullptr : m_vpCameras.at(m_activeCamera); }
	/**
	 * @brief Checks intersection of ray \b ray with all contained objects
	 * @param ray The ray
	 * @retval true If ray \b ray intersects any object
	 * @retval false otherwise
	 */
	bool intersect(Ray& ray) const
	{
		bool hit = false;
		for (auto prim = m_vpPrims.begin(); prim != m_vpPrims.end(); prim++) {
			if (prim->get()->intersect(ray))
				hit = true;
		}
		return hit;
	}

	/**
	 * takes ray pointing to light source and returns whether there is an
	 * object between its orgin and the source
	 */
	bool occluded(Ray& ray)
	{
		
		//get dist to nearest object
		Ray ray2;
		ray2.org = ray.org;
		ray2.dir = ray.dir;
		intersect(ray2);
		
		return (ray.t > ray2.t);
	}

	/**
	 trace the given ray and shade it and
	 return the color of the shaded ray
	 */
	Vec3f RayTrace(Ray& ray) const
	{
		if (intersect(ray))
			return ray.hit->getShader()->shade(ray);
		return m_bgColor;
	}


private:
	Vec3f						m_bgColor;    			///< background color
	std::vector<ptr_prim_t> 	m_vpPrims;				///< primitives
	std::vector<ptr_light_t>	m_vpLights;				///< lights
	std::vector<ptr_camera_t>	m_vpCameras;			///< Cameras
	size_t						m_activeCamera	= 0;	///< The index of the active camera
};
