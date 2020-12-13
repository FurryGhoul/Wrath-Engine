#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "Glew/include/glew.h"
#include "Application.h"

#include "MathGeoLib/Geometry/Plane.h"

ComponentCamera::ComponentCamera(Component_Type type, GameObject* parent) : Component(type, parent)
{
	camera_frustum.type = math::FrustumType::PerspectiveFrustum;

	camera_frustum.pos = { 0.0f, 4.0f, 10.0f };
	camera_frustum.front = { 0.0f,0.0f,-1.0f };
	camera_frustum.up = { 0.0f,1.0f,0.0f };

	camera_frustum.nearPlaneDistance = 0.1f;
	camera_frustum.farPlaneDistance = 500.0f;

	camera_frustum.verticalFov = DEGTORAD * 90.0f;
	SetAspectRatio(App->window->width / App->window->height);

	CalculateAABB();

	//IsActive = false;
}

ComponentCamera::~ComponentCamera()
{}

bool ComponentCamera::ComponentUpdate()
{
	if (parent && (ComponentTransform*)parent->GetComponent(TRANSFORM))
	{
		camera_frustum.SetWorldMatrix(((ComponentTransform*)parent->GetComponent(TRANSFORM))->GetGlobalMatrix().Float3x4Part());
	}
	return true;
}


void ComponentCamera::SetAspectRatio(float aspect_ratio)
{
	camera_frustum.horizontalFov = 2.f * atanf(tanf(camera_frustum.verticalFov * 0.5f) * aspect_ratio);
}

float* ComponentCamera::GetViewMatrix()
{
	static float4x4 matrix;
	matrix = camera_frustum.ViewMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

float* ComponentCamera::GetProjectionMatrix()
{
	static float4x4 matrix;
	matrix = camera_frustum.ProjectionMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

void ComponentCamera::CalculateAABB()
{
	float3 corner_points[8];
	camera_frustum.GetCornerPoints(corner_points);

	camera_BB.SetNegativeInfinity();
	camera_BB.Enclose(corner_points, 8);
}

bool ComponentCamera::ContainsAABB(const AABB& ref_box)
{
	float3 corners[8];
	math::Plane planes[6];
	int iTotalIn = 0;

	ref_box.GetCornerPoints(corners);
	camera_frustum.GetPlanes(planes);

	for (int p = 0; p < 6; ++p)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (!planes[p].IsOnPositiveSide(corners[i]))
			{
				++iTotalIn;
				break;
			}
		}
		if (iTotalIn == 0)
		{
			return false;
		}
	}

	if (iTotalIn == 6)
	{
		return true;
	}

	return false;

}

void ComponentCamera::DrawRay()
{
	float3 raystart = App->scene->ray.a;
	float3 rayend = App->scene->ray.b;

	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(raystart.x, raystart.y, raystart.z);
	glVertex3f(rayend.x, rayend.y, rayend.z);

	glEnd();

	glLineWidth(1.0f);
}

void ComponentCamera::DrawCameraBoundingBox(const AABB& boundingbox)
{
	glLineWidth(2.f);
	glColor3f(255, 255, 255);

	glBegin(GL_LINES);

	glVertex3f(boundingbox.CornerPoint(0).x, boundingbox.CornerPoint(0).y, boundingbox.CornerPoint(0).z);
	glVertex3f(boundingbox.CornerPoint(1).x, boundingbox.CornerPoint(1).y, boundingbox.CornerPoint(1).z);

	glVertex3f(boundingbox.CornerPoint(0).x, boundingbox.CornerPoint(0).y, boundingbox.CornerPoint(0).z);
	glVertex3f(boundingbox.CornerPoint(2).x, boundingbox.CornerPoint(2).y, boundingbox.CornerPoint(2).z);

	glVertex3f(boundingbox.CornerPoint(0).x, boundingbox.CornerPoint(0).y, boundingbox.CornerPoint(0).z);
	glVertex3f(boundingbox.CornerPoint(4).x, boundingbox.CornerPoint(4).y, boundingbox.CornerPoint(4).z);

	glVertex3f(boundingbox.CornerPoint(3).x, boundingbox.CornerPoint(3).y, boundingbox.CornerPoint(3).z);
	glVertex3f(boundingbox.CornerPoint(1).x, boundingbox.CornerPoint(1).y, boundingbox.CornerPoint(1).z);

	glVertex3f(boundingbox.CornerPoint(3).x, boundingbox.CornerPoint(3).y, boundingbox.CornerPoint(3).z);
	glVertex3f(boundingbox.CornerPoint(2).x, boundingbox.CornerPoint(2).y, boundingbox.CornerPoint(2).z);

	glVertex3f(boundingbox.CornerPoint(3).x, boundingbox.CornerPoint(3).y, boundingbox.CornerPoint(3).z);
	glVertex3f(boundingbox.CornerPoint(7).x, boundingbox.CornerPoint(7).y, boundingbox.CornerPoint(7).z);

	glVertex3f(boundingbox.CornerPoint(6).x, boundingbox.CornerPoint(6).y, boundingbox.CornerPoint(6).z);
	glVertex3f(boundingbox.CornerPoint(2).x, boundingbox.CornerPoint(2).y, boundingbox.CornerPoint(2).z);

	glVertex3f(boundingbox.CornerPoint(6).x, boundingbox.CornerPoint(6).y, boundingbox.CornerPoint(6).z);
	glVertex3f(boundingbox.CornerPoint(4).x, boundingbox.CornerPoint(4).y, boundingbox.CornerPoint(4).z);

	glVertex3f(boundingbox.CornerPoint(6).x, boundingbox.CornerPoint(6).y, boundingbox.CornerPoint(6).z);
	glVertex3f(boundingbox.CornerPoint(7).x, boundingbox.CornerPoint(7).y, boundingbox.CornerPoint(7).z);

	glVertex3f(boundingbox.CornerPoint(5).x, boundingbox.CornerPoint(5).y, boundingbox.CornerPoint(5).z);
	glVertex3f(boundingbox.CornerPoint(1).x, boundingbox.CornerPoint(1).y, boundingbox.CornerPoint(1).z);

	glVertex3f(boundingbox.CornerPoint(5).x, boundingbox.CornerPoint(5).y, boundingbox.CornerPoint(5).z);
	glVertex3f(boundingbox.CornerPoint(4).x, boundingbox.CornerPoint(4).y, boundingbox.CornerPoint(4).z);

	glVertex3f(boundingbox.CornerPoint(5).x, boundingbox.CornerPoint(5).y, boundingbox.CornerPoint(5).z);
	glVertex3f(boundingbox.CornerPoint(7).x, boundingbox.CornerPoint(7).y, boundingbox.CornerPoint(7).z);

	glEnd();
	glColor3f(1, 1, 1);
	glLineWidth(1.0f);
}

void ComponentCamera::DrawFrustrum()
{
	float3 points[8];
	camera_frustum.GetCornerPoints(points);

	glLineWidth(2.0f);
	glBegin(GL_LINES);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[1].x, points[1].y, points[1].z);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[2].x, points[2].y, points[2].z);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[4].x, points[4].y, points[4].z);

	glVertex3f(points[1].x, points[1].y, points[1].z);
	glVertex3f(points[3].x, points[3].y, points[3].z);

	glVertex3f(points[1].x, points[1].y, points[1].z);
	glVertex3f(points[5].x, points[5].y, points[5].z);

	glVertex3f(points[2].x, points[2].y, points[2].z);
	glVertex3f(points[3].x, points[3].y, points[3].z);

	glVertex3f(points[2].x, points[2].y, points[2].z);
	glVertex3f(points[6].x, points[6].y, points[6].z);

	glVertex3f(points[3].x, points[3].y, points[3].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glVertex3f(points[4].x, points[4].y, points[4].z);
	glVertex3f(points[5].x, points[5].y, points[5].z);

	glVertex3f(points[4].x, points[4].y, points[4].z);
	glVertex3f(points[6].x, points[6].y, points[6].z);

	glVertex3f(points[5].x, points[5].y, points[5].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glVertex3f(points[6].x, points[6].y, points[6].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glEnd();

	glColor3f(1, 1, 1);
	glLineWidth(1.0f);
}