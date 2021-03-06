#include <iostream>
#include <cmath>
#include "catch.hpp"
#include "../src/Systems/Physics/Grid.hpp"
#include "../src/Systems/Physics/ColliderBuilder.hpp"
#include "../src/util.hpp"

TEST_CASE("Grid Test")
{
	Grid grid(200.f, 5.f);
	const std::vector< std::vector<Cell> >& cells = grid.cells;

	REQUIRE(cells.size() == 20);
	REQUIRE(cells[0].size() == 20);

	std::vector<glm::vec3> points1;
	points1.push_back(glm::vec3(1.f, 1.f, 1.f));
	points1.push_back(glm::vec3(1.f, 3.f, 1.f));
	points1.push_back(glm::vec3(3.f, 3.f, 3.f));
	points1.push_back(glm::vec3(3.f, 1.f, 3.f));

	points1.push_back(glm::vec3(1.f, 1.f, 2.f));
	points1.push_back(glm::vec3(1.f, 3.f, 2.f));
	points1.push_back(glm::vec3(3.f, 3.f, 4.f));
	points1.push_back(glm::vec3(3.f, 1.f, 4.f));
	std::shared_ptr<Collider> collider1 = ColliderBuilder::Build(1, DynamicType::Static, points1);

	std::vector<glm::vec3> points2;
	points2.push_back(glm::vec3(0.f, 0.f, 10.f));
	points2.push_back(glm::vec3(5.f, 0.f, 10.f));
	points2.push_back(glm::vec3(5.f, 0.f, 20.f));
	points2.push_back(glm::vec3(0.f, 0.f, 20.f));

	points2.push_back(glm::vec3(0.f, 0.f, 11.f));
	points2.push_back(glm::vec3(5.f, 0.f, 11.f));
	points2.push_back(glm::vec3(5.f, 0.f, 21.f));
	points2.push_back(glm::vec3(0.f, 0.f, 21.f));
	std::shared_ptr<Collider> collider2 = ColliderBuilder::Build(2, DynamicType::Static, points2);

	std::vector<glm::vec3> points3;
	points3.push_back(glm::vec3(2.f, 0.f, 0.f));
	points3.push_back(glm::vec3(2.f, 0.f, 4.f));
	points3.push_back(glm::vec3(2.f, 4.f, 0.f));
	points3.push_back(glm::vec3(2.f, 4.f, 4.f));

	points3.push_back(glm::vec3(6.f, 0.f, 0.f));
	points3.push_back(glm::vec3(6.f, 0.f, 4.f));
	points3.push_back(glm::vec3(6.f, 4.f, 0.f));
	points3.push_back(glm::vec3(6.f, 4.f, 4.f));
	std::shared_ptr<Collider> collider3 = ColliderBuilder::Build(3, DynamicType::Dynamic, points3);

	std::vector<glm::vec3> points4;
	points4.push_back(glm::vec3(15.f, 0.f, 0.f));
	points4.push_back(glm::vec3(15.f, 5.f, 0.f));
	points4.push_back(glm::vec3(15.f, 0.f, 5.f));
	points4.push_back(glm::vec3(15.f, 5.f, 5.f));

	points4.push_back(glm::vec3(20.f, 0.f, 0.f));
	points4.push_back(glm::vec3(20.f, 5.f, 0.f));
	points4.push_back(glm::vec3(20.f, 0.f, 5.f));
	points4.push_back(glm::vec3(20.f, 5.f, 5.f));
	std::shared_ptr<Collider> collider4 = ColliderBuilder::Build(4, DynamicType::Dynamic, points4);

	std::vector<glm::vec3> points5;
	points5.push_back(glm::vec3(15.f, 1.f, 4.f));
	points5.push_back(glm::vec3(15.f, 6.f, 4.f));
	points5.push_back(glm::vec3(15.f, 1.f, 9.f));
	points5.push_back(glm::vec3(15.f, 6.f, 9.f));

	points5.push_back(glm::vec3(20.f, 1.f, 4.f));
	points5.push_back(glm::vec3(20.f, 6.f, 4.f));
	points5.push_back(glm::vec3(20.f, 1.f, 9.f));
	points5.push_back(glm::vec3(20.f, 6.f, 9.f));
	std::shared_ptr<Collider> collider5 = ColliderBuilder::Build(5, DynamicType::Dynamic, points5);

	grid.Insert(collider1);
	grid.Insert(collider2);
	grid.Insert(collider3);
	grid.Insert(collider4);
	grid.Insert(collider5);

	SECTION("Test insertion")
	{
		Cell& cell1 = grid.cells[0][0];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders1 = cell1.GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders1 = cell1.GetStaticColliders();
		REQUIRE(dynamicColliders1.size() == 1);
		REQUIRE(staticColliders1.size() == 1);

		Cell& cell2 = grid.cells[1][0];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders2 = cell2.GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders2 = cell2.GetStaticColliders();
		REQUIRE(dynamicColliders2.size() == 0);
		REQUIRE(staticColliders2.size() == 1);

		Cell& cell3 = grid.cells[0][1];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders3 = cell3.GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders3 = cell3.GetStaticColliders();
		REQUIRE(dynamicColliders3.size() == 2);
		REQUIRE(staticColliders3.size() == 0);

		Cell& cell4 = grid.cells[1][1];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders4 = cell4.GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders4 = cell4.GetStaticColliders();
		REQUIRE(dynamicColliders4.size() == 0);
		REQUIRE(staticColliders4.size() == 0);

	}

	SECTION("Test Removal")
	{
		grid.Remove(collider1);
		grid.Remove(collider3);
		Cell& cell1 = grid.cells[0][0];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders1 = cell1.GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders1 = cell1.GetStaticColliders();
		REQUIRE(dynamicColliders1.size() == 0);
		REQUIRE(staticColliders1.size() == 0);
	}

	SECTION("Test GetEligibleCells")
	{
		std::vector<std::pair<int, int>> eligibleCells = grid.GetEligibleCells(0, 0);
		REQUIRE(eligibleCells.size() == 4);
		std::vector<std::pair<int, int>> eligibleCells1 = grid.GetEligibleCells(4, 4);
		REQUIRE(eligibleCells1.size() == 6);
		std::vector<std::pair<int, int>> eligibleCells2 = grid.GetEligibleCells(19, 19);
		REQUIRE(eligibleCells2.size() == 2);
		std::vector<std::pair<int, int>> eligibleCells3 = grid.GetEligibleCells(19, 0);
		REQUIRE(eligibleCells3.size() == 4);
	}
}