/*  The following code is a VERY heavily modified from code originally sourced from:
Ray tracing tutorial of http://www.codermind.com/articles/Raytracer-in-C++-Introduction-What-is-ray-tracing.html
It is free to use for educational purpose and cannot be redistributed outside of the tutorial pages. */

#include "Lighting.h"
#include "Colour.h"
#include "Intersection.h"
#include "Texturing.h"

// test to see if light ray collides with any of the scene's objects
// short-circuits when first intersection discovered, because no matter what the object will be in shadow
bool isInShadow(const Scene* scene, const Ray* lightRay, const float lightDist)
{
	float t = lightDist;
		// search for sphere collision
		if (isSphereIntersected(scene, lightRay, t)) return true;

		// search for box collision

		if (isBoxIntersectedShadow(scene, lightRay, &t))
		{
			return true;
		}

	
	// not in shadow
	return false;
}


//// apply diffuse lighting with respect to material's colouring
//Colour applyDiffuse(const Ray* lightRay, const Light* currentLight, const Intersection* intersect)
//{
//	Colour output;
//
//
//	switch (intersect->material->type)
//	{
//	case Material::GOURAUD:
//		output = intersect->material->diffuse;
//		break;
//	case Material::CHECKERBOARD:
//			Point p = (intersect->pos - intersect->material->offset) / intersect->material->size;
//
//			int which = (int(floorf(p.x)) + int(floorf(p.y)) + int(floorf(p.z))) & 1;
//
//			output = (which ? intersect->material->diffuse : intersect->material->diffuse2);
//		break;
//	case Material::CIRCLES:
//		// apply computed circular texture
//			Point p = (intersect->pos - intersect->material->offset) / intersect->material->size;
//
//			int which = int(floorf(sqrtf(p.x * p.x + p.y * p.y + p.z * p.z))) & 1;
//
//			output = (which ? intersect->material->diffuse : intersect->material->diffuse2);
//
//		break;
//	case Material::WOOD:
//		// apply computed wood grain texture
//
//			Point p = (intersect->pos - intersect->material->offset) / intersect->material->size;
//
//			// squiggle up where the point is
//			p = { p.x * cosf(p.y * 0.996f) * sinf(p.z * 1.023f), cosf(p.x) * p.y * sinf(p.z * 1.211f), cosf(p.x * 1.473f) * cosf(p.y * 0.795f) * p.z };
//
//			int which = int(floorf(sqrtf(p.x * p.x + p.y * p.y + p.z * p.z))) & 1;
//
//			 output = (which ? intersect->material->diffuse : intersect->material->diffuse2);
//
//		break;
//	}
//
//	float lambert = lightRay->dir * intersect->normal;
//
//	return lambert * currentLight->intensity * output;
//}


// apply diffuse lighting with respect to material's colouring
Vector8 applyDiffuse(Vector8 lightRay, Vector8 currentLight, const Intersection* intersect)
{
	// set variables
	Colour output;
	Vector8 outputs;
	Point p;
	Vector8 norms(intersect->normal.x, intersect->normal.y, intersect->normal.z);
	int which;

	// switch statement
	// shouldnt need to be translated to SIMD because it only calculates a singular value meaning that simd implementation would be wasted
	switch (intersect->material->type)
	{
	case Material::GOURAUD:
		output = intersect->material->diffuse;
		break;
	case Material::CHECKERBOARD:
		p = (intersect->pos - intersect->material->offset) / intersect->material->size;

		which = (int(floorf(p.x)) + int(floorf(p.y)) + int(floorf(p.z))) & 1;

		output = (which ? intersect->material->diffuse : intersect->material->diffuse2);
		break;
	case Material::CIRCLES:
		// apply computed circular texture
		p = (intersect->pos - intersect->material->offset) / intersect->material->size;

		which = int(floorf(sqrtf(p.x * p.x + p.y * p.y + p.z * p.z))) & 1;

		output = (which ? intersect->material->diffuse : intersect->material->diffuse2);

		break;
	case Material::WOOD:
		// apply computed wood grain texture

		Point p = (intersect->pos - intersect->material->offset) / intersect->material->size;

		// squiggle up where the point is
		p = { p.x * cosf(p.y * 0.996f) * sinf(p.z * 1.023f), cosf(p.x) * p.y * sinf(p.z * 1.211f), cosf(p.x * 1.473f) * cosf(p.y * 0.795f) * p.z };

		which = int(floorf(sqrtf(p.x * p.x + p.y * p.y + p.z * p.z))) & 1;

		output = (which ? intersect->material->diffuse : intersect->material->diffuse2);

		break;
	}
	// set SIMD variable to be the lightray multiplied by a vector8 normal
	__m256 lambert = dot(lightRay, norms);

	// stage to convert the coutputs to a vector 8
	Vector8 outputconv(output.red, output.green, output.blue);

	// set the return Vector 8 value to be the xyz multiplied by the lambert value.
	outputs = { (lambert * currentLight.xs * outputconv.xs), (lambert * currentLight.ys * outputconv.ys), (lambert * currentLight.zs * outputconv.zs) };

	// return Vector 8
	return outputs;
}


// Blinn 
// The direction of Blinn is exactly at mid point of the light ray and the view ray. 
// We compute the Blinn vector and then we normalize it then we compute the coeficient of blinn
// which is the specular contribution of the current light.

Vector8 applySpecular(Vector8 rayDir, Vector8 lightColour, __m256 fLightProjection, const Ray* viewRay, const Intersection* intersect)
{
	// set variables
	Vector8 blinnDir;
	Vector8 viewRayDir(viewRay->dir.x, viewRay->dir.y, viewRay->dir.z);

	// set blinDir to be the light direction minus the view ray directions
	blinnDir = rayDir - viewRayDir;

	// set blinn SIMD to be the inverse square root of blinnDir^2 multiplied by the max of either fLightproject minus the intersect viewProject or zero
	__m256 blinn = _mm256_invsqrt_ps(dot(blinnDir, blinnDir)) * _mm256_max_ps((fLightProjection - _mm256_set1_ps(intersect->viewProjection)), _mm256_set1_ps(0.0f));

	// set blinn to be a power of itself by the material power
	blinn = _mm256_pow_ps(blinn, _mm256_set1_ps(intersect->material->power));

	// set the specular to be a vector 8 of the intersect material specular colours
	Vector8 spec = { intersect->material->specular.red, intersect->material->specular.green, intersect->material->specular.blue };

	// combine the specular colour and the light colour
	Vector8 specLight = { spec.xs * lightColour.xs, spec.ys * lightColour.ys, spec.zs * lightColour.zs };

	// set vecotr 8 returned value to be the spec light value multiplied by the blinn value
	Vector8 returned = { blinn * specLight.xs, blinn * specLight.ys, blinn * specLight.zs };

	// return the vector8
	return returned;
}


Colour applyLighting(const Scene* scene, const Ray* viewRay, const Intersection* intersect)
{
	// colour to return (starts as black)
	Colour output(0.0f, 0.0f, 0.0f);

	Vector8 outputs(0.0f, 0.0f, 0.0f);

	// same starting point for each light ray
	Ray lightRay = { intersect->pos };

	// make a currentLight
	Light currentLight;

	//constant Vector 8 variables
	Vector8 interPos(intersect->pos.x, intersect->pos.y, intersect->pos.z);
	Vector8 rayDir(lightRay.dir.x, lightRay.dir.y, lightRay.dir.z);
	Vector8 rayStart(intersect->pos.x, intersect->pos.y, intersect->pos.z);
	Vector8 interNorm(intersect->normal.x, intersect->normal.y, intersect->normal.z);

	//constant SIMD variables
	__m256i bools = _mm256_set1_epi32(0);
	__m256i val = _mm256_set1_epi32(0);
	__m256i simdCheck = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
	__m256i numLightSIMDVal = _mm256_set1_epi32(scene->numLightsSIMD - 1);
	__m256i numLightVal = _mm256_set1_epi32((scene->numLights % 8));

	// loop through all the lights
	for (unsigned int i = 0; i < scene->numLightsSIMD; ++i)
	{
		// create Vector8 for light positions and colours
		Vector8 lightColour(scene->red[i], scene->green[i], scene->blue[i]);
		Vector8 lightPos(scene->lightX[i], scene->lightY[i], scene->lightZ[i]);

		// set the light direction to be the light position minus the intersect position
		rayDir = lightPos - interPos;

		// set SIMD variable for i
		__m256i iVal = _mm256_set1_epi32(i);

		// check if on last simdlight SIMD
		__m256i numLightsCom = _mm256_and_si256(iVal, numLightSIMDVal);

		//check if the number of lights in the current simd is less than the index at each point in the SIMD
		__m256i numLightsMask = _mm256_cmpgt_epi32(numLightVal, simdCheck);

		// join the test cases together
		__m256i tempcheck = _mm256_and_si256(numLightsCom, numLightsMask);




		// multiply the ray direction with the intersect normal
		__m256 angleBetweenLightAndNormalSIMD = dot(rayDir, interNorm);
		//		float angleBetweenLightAndNormal = lightRay.dir * intersect->normal;

		// check if the angle is greater than 0
		__m256 checkangle = _mm256_cmp_ps(angleBetweenLightAndNormalSIMD, _mm256_set1_ps(0.0f), _CMP_GT_OQ);

		// if it is the last light then only use relevant lights else run full number of lights
		__m256 numLightsFin = select(_mm256_castsi256_ps(numLightsCom), _mm256_castsi256_ps(tempcheck), checkangle);
		
		// combine tests
		__m256 check = _mm256_and_ps(checkangle, numLightsFin);

		// simd version of calc below
		__m256 lightDistSIMD = _mm256_sqrt_ps(dot(rayDir, rayDir));
		//		float lightDist = sqrtf(lightRay.dir.dot());

		// simd version of calc below
		__m256 invLightDistSIMD = _mm256_set1_ps(1.0f) / lightDistSIMD;
		// float invLightDist = 1.0f / lightDist;

		// simd version of calc below
		__m256 lightProjectionSIMD = invLightDistSIMD * angleBetweenLightAndNormalSIMD;
		// 		float lightProjection = invLightDist * angleBetweenLightAndNormal;

		// set direction to be itself multiplied by the inverse light dist
		rayDir.xs = rayDir.xs * invLightDistSIMD;
		rayDir.ys = rayDir.ys * invLightDistSIMD;
		rayDir.zs = rayDir.zs * invLightDistSIMD;

		// loop for isinshadow
		for (unsigned int j = 0; j < 8; ++j)
		{
			// code to get current light intensity for isInSHadow
			currentLight.intensity.red = lightColour.xs.m256_f32[j];
			currentLight.intensity.green = lightColour.ys.m256_f32[j];
			currentLight.intensity.blue = lightColour.zs.m256_f32[j];

			// code to get current light pos for isInSHadow
			currentLight.pos.x = lightPos.xs.m256_f32[j];
			currentLight.pos.y = lightPos.ys.m256_f32[j];
			currentLight.pos.z = lightPos.zs.m256_f32[j];

			// light direction based on intersect
			lightRay.dir = currentLight.pos - intersect->pos;

			// light direction based on invLightDistSIMD
			lightRay.dir = lightRay.dir * invLightDistSIMD.m256_f32[j];

			// call is in shadow and store bool in __m256
			bools.m256i_i32[j] = (isInShadow(scene, &lightRay, lightDistSIMD.m256_f32[j]));

			// convert scalar bool to be SIMD bool
			val = _mm256_cmpeq_epi32(bools, _mm256_set1_epi32(1));

		}
		// add diffuse lighting from colour / texture
		Vector8 applyDiff = outputs + applyDiffuse(rayDir, lightColour, intersect);

		// check if no in shadow from val
		__m256 cond1 = _mm256_cmp_ps(_mm256_cvtepi32_ps(val), _mm256_set1_ps(0.0f), _CMP_EQ_OQ);

		// combine check from angleBetweenLightAndNormalSIMD
		__m256 check2 = _mm256_and_ps(check, cond1);

		// set outputs based on check2
		outputs.xs = select(check2, applyDiff.xs, outputs.xs);
		outputs.ys = select(check2, applyDiff.ys, outputs.ys);
		outputs.zs = select(check2, applyDiff.zs, outputs.zs);

		// add specular from light direction, colour and projection 
		Vector8 applySpec = outputs + applySpecular(rayDir, lightColour, lightProjectionSIMD, viewRay, intersect);

		// set outputs based on check2
		outputs.xs = select(check2, applySpec.xs, outputs.xs);
		outputs.ys = select(check2, applySpec.ys, outputs.ys);
		outputs.zs = select(check2, applySpec.zs, outputs.zs);
		
	}

	// set permute for hAdd
	__m256i permuteVal = _mm256_setr_epi32(1, 4, 1, 4, 1, 4, 1, 4);

	// horizontally add the rgb of the outputs so the function can return a normal vector
	__m256 hAdd1r = _mm256_hadd_ps(outputs.xs, outputs.xs);
	__m256 hAdd2r = _mm256_hadd_ps(hAdd1r, hAdd1r);
	__m256 permuteR = _mm256_permutevar8x32_ps(hAdd2r, permuteVal);
	__m256 hAdd3r = _mm256_hadd_ps(permuteR, permuteR);

	__m256 hAdd1g = _mm256_hadd_ps(outputs.ys, outputs.ys);
	__m256 hAdd2g = _mm256_hadd_ps(hAdd1g, hAdd1g);
	__m256 permuteg = _mm256_permutevar8x32_ps(hAdd2g, permuteVal);
	__m256 hAdd3g = _mm256_hadd_ps(permuteg, permuteg);

	__m256 hAdd1B = _mm256_hadd_ps(outputs.zs, outputs.zs);
	__m256 hAdd2B = _mm256_hadd_ps(hAdd1B, hAdd1B);
	__m256 permuteB = _mm256_permutevar8x32_ps(hAdd2B, permuteVal);
	__m256 hAdd3B = _mm256_hadd_ps(permuteB, permuteB);
	
	// set the vector output
	output = { hAdd3r.m256_f32[0], hAdd3g.m256_f32[0], hAdd3B.m256_f32[0] };

	// return output
	return output;
}


