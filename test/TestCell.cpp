#include "catch.hpp"

#include "../src/Systems/Physics/Cell.hpp"
#include "../src/Systems/Physics/ColliderBuilder.hpp"

TEST_CASE("Test Cell")
{
	Cell cell(glm::vec3(10.f,10.f,10.f), 5.f, 1, 1);
	std::vector<std::pair<glm::vec3, glm::vec3>> edges1;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces1;
	glm::vec3 center1;
	std::vector<glm::vec3> points1;
	points1.push_back(glm::vec3(-1.f, -0.1f, 0.05f));
	points1.push_back(glm::vec3(4.f, -0.1f, 0.05f));
	points1.push_back(glm::vec3(-1.f, 4.f, 0.05f));
	points1.push_back(glm::vec3(4.f, 4.f, 0.05f));

	points1.push_back(glm::vec3(-1.f, -0.1f, -0.2f));
	points1.push_back(glm::vec3(4.f, -0.1f, -0.2f));
	points1.push_back(glm::vec3(-1.f, 4.f, -0.2f));
	points1.push_back(glm::vec3(4.f, 4.f, -0.2f));
	ColliderBuilder::Build(points1, faces1, edges1, center1);
	std::shared_ptr<Collider> collider1 = std::make_shared<Collider>(1, center1, points1, edges1, faces1, DynamicType::Static);

	std::vector<std::pair<glm::vec3, glm::vec3>> edges3;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces3;
	glm::vec3 center3;
	std::vector<glm::vec3> points3;
	points3.push_back(glm::vec3(3.f, 1.f, 1.f));
	points3.push_back(glm::vec3(0.9f, -1.f, 1.f));
	points3.push_back(glm::vec3(2.f, -2.f, 1.f));
	points3.push_back(glm::vec3(4.f, 0.f, 1.f));

	points3.push_back(glm::vec3(3.f, 1.f, 0.f));
	points3.push_back(glm::vec3(0.9f, -1.f, 0.f));
	points3.push_back(glm::vec3(2.f, -2.f, 0.f));
	points3.push_back(glm::vec3(4.f, 0.f, 0.f));
	ColliderBuilder::Build(points3, faces3, edges3, center3);
	std::shared_ptr<Collider> collider3 = std::make_shared<Collider>(3, center3, points3, edges3, faces3, DynamicType::Dynamic);

	std::vector<std::pair<glm::vec3, glm::vec3>> edges4;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces4;
	glm::vec3 center4;
	std::vector<glm::vec3> points4;
	points4.push_back(glm::vec3(0.f, 0.f, 0.f));
	points4.push_back(glm::vec3(2.f, 0.f, 0.f));
	points4.push_back(glm::vec3(2.f, 2.f, 0.f));
	points4.push_back(glm::vec3(0.f, 2.f, 0.f));

	points4.push_back(glm::vec3(0.f, 0.f, 2.f));
	points4.push_back(glm::vec3(2.f, 0.f, 2.f));
	points4.push_back(glm::vec3(2.f, 2.f, 2.f));
	points4.push_back(glm::vec3(0.f, 2.f, 2.f));
	
	ColliderBuilder::Build(points4, faces4, edges4, center4);
	std::shared_ptr<Collider> collider4 = std::make_shared<Collider>(4, center4, points4, edges4, faces4, DynamicType::Dynamic);

	cell.Insert(collider1);
	cell.Insert(collider3);
	cell.Insert(collider4);
	REQUIRE(cell.GetDynamicColliders().size() == 2);
	REQUIRE(cell.GetStaticColliders().size() == 1);
	cell.Remove(collider3);
	REQUIRE(cell.GetStaticColliders().size() == 1);
	REQUIRE(cell.GetDynamicColliders().size() == 1);
}