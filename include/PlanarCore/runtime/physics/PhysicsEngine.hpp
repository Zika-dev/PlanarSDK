#pragma once

#include <unordered_map>
#include <optional>
#include <set>
#include <limits>

#include "Core.hpp"
#include "Log.hpp"
#include "runtime/SystemBase.hpp"
#include "SpatialHash.hpp"
#include "Tiled/ObjectLayer.hpp"
#include "graphics/library/Camera.hpp"
#include "inplace_function.h"
#include "runtime/Tag.hpp"

namespace core::runtime {
	class PLANAR_API PhysicsEngine : public SystemBase {

	public:
		enum class ForceMode {
			Force,
			Impulse,
			VelocityChange,
		};

		struct PhysicsWorld;
		struct RigidBody;
		struct RectCollider;

		struct PhysicsObjectRef {
			int vectorIndex = -1;
			bool isDynamic = false;
		};

		struct CollisionData {

			PhysicsObjectRef object;

			Vec2<float> normal;
			Vec2<float> depth;
			Vec2<float> relativeVelocity;

			bool isTrigger = false;
			bool stepped = false;

			bool wasStatic() const noexcept {
				return !object.isDynamic;
			}

			int32_t getObject() const {
				if (wasStatic()) return -1;
				return worldPtr->dynamicBodies[object.vectorIndex].rb.objectId;
			}

			RigidBody* getRigidbody() const {
				if (wasStatic()) return nullptr;
				return &worldPtr->dynamicBodies[object.vectorIndex].rb;
			}

			RectCollider* getCollider() const {
				return object.isDynamic ? &worldPtr->dynamicBodies[object.vectorIndex].collider :
					&worldPtr->staticColliders[std::numeric_limits<int>::max() - object.vectorIndex];
			}

		private:

			PhysicsWorld* worldPtr;

			bool collision = false;
			float stepDist = std::numeric_limits<float>::max();

			friend class PhysicsEngine;
		};

		// Components
		struct RigidBody {

			RigidBody() = default;
			RigidBody(int id) : objectId(id) {}

			float restitution = 0.0f; // 1 = perfect bounce, 0 = no bounce
			float friction = 0.5f; // 1 = full friction, 0 = no friction,
			float gravityScale = 1.0f;
			float maxStep = 10.0f;
			bool isGrounded = false;

			Vec2<float> velocity{};
			Vec2<float> position{};

			Vec2<float> netForce{};
			bool fixed = false;
			bool pushable = true; // If false, object will not receive forces from collisions but will still resolve collisions

			void addForce(Vec2<float> force, ForceMode mode) {
				switch (mode)
				{
				case ForceMode::Force:
					netForce = netForce + force;
					break;
				case ForceMode::Impulse:
					velocity = velocity + force * invMass;
					break;
				case ForceMode::VelocityChange:
					velocity = velocity + force;
					break;
				default:
					break;
				}
			}

			void setMass(float mass) noexcept {
				invMass = 1.0f / mass;
			}

			float getMass() const noexcept {
				return 1.0f / invMass;
			}

			// Callbacks
			stdext::inplace_function<void(CollisionData), 32> onEnter;
			stdext::inplace_function<void(CollisionData), 32> onExit;

		private:

			void updatePos(float dt) {
				position = position + velocity * dt;
			}

			void updateVelocity(float dt, float gravity) {
				Vec2<float> acceleration = { 
					netForce.x * invMass, 
					netForce.y * invMass + gravity * gravityScale
				};
				velocity = velocity + acceleration * dt;
			}

			int objectId = -1;

			// Index into vector
			uint32_t index = 0;

			float invMass = 1.0f;

			// Collisions
			std::unordered_map<int, CollisionData> currentCollisions;  // Collider ID : CollisionData
			std::unordered_map<int, CollisionData> previousCollisions; // Collider ID : CollisionData

			Vec2<float> frameMaxPosCorrection; // max correction applied in +x/+y this frame
			Vec2<float> frameMaxNegCorrection; // max correction applied in -x/-y this frame

			friend class PhysicsEngine;
		};

		struct LayerMask {
			LayerMask() = default;

			LayerMask(uint32_t bit, uint32_t mask) {
				layer |= (1 << bit);
				this->mask = mask;
			}

			uint32_t layer{}; // This layer
			uint32_t mask{};  // The layers this layer masks (layer mask)
		};

		struct RectCollider {

			// Identity
			LayerMask layerMask;
			Tag tag;

			int left = 0;
			int top = 0;
			int right = 0;
			int bottom = 0;

			bool isTrigger = false;

			RectCollider(int left, int top, int right, int bottom) : left(left), top(top), right(right), bottom(bottom) { }

			RectCollider() : left(0), top(0), right(0), bottom(0) { };

			Vec2<float> getMidpoint() const {
				return { left + 0.5f * (right - left), top + 0.5f * (bottom - top) };
			}

			void addPosition(Vec2<int> pos) {
				left += pos.x;
				right += pos.x;
				top += pos.y;
				bottom += pos.y;
			}

		private:
			int id = -1;

			friend class PhysicsEngine;
		};

		// Has both a rigid body and a collider
		struct DynamicBody {
			RigidBody rb;
			RectCollider collider;
		};

		// General object, can be a dynamic body or a static collider
		struct PhysicsObject {
			std::optional<RigidBody> rb;
			RectCollider collider;
		};

		// Debugging
		struct PhysicsData {
			size_t numRigidBodies = 0;
			size_t numColliders = 0;

			size_t numBroadPhase = 0;
			size_t numNarrowPhase = 0;

			// us
			float hashBuildTime = 0;
			float broadPhaseTime = 0;
			float narrowPhaseTime = 0;
			float contactTime = 0;
			float callbackCollectTime = 0;
			float stepTime = 0;

			float rayTraverseTime = 0;
			float raySlabTime = 0;
			float raySortTime = 0;
			float rayTotalTime = 0;
		};

		struct RayCastDebugData {
			Vec2<int> start, end;
			float age{};
		};

		struct WorldConfig {
			uint32_t maxObjects = 512;
			uint32_t cellSize = 100;

			float broadPhaseRadius = 75.0f;

			float gravity = 300.0f;

			uint32_t updateFrequency = 60; // 60 Hz
		};

		struct PhysicsWorld {

			PhysicsWorld(WorldConfig cfg = {}) :
			hashGrid(cfg.cellSize, cfg.maxObjects * 4, cfg.maxObjects * 4),
			gravity(cfg.gravity),
			updateFrequency(cfg.updateFrequency)
			{
				coordsCache.resize(cfg.maxObjects * 4);
				dynamicBodies.reserve(cfg.maxObjects);

				valid = true;
				stepTime = 1.0f / updateFrequency;
			}

			float gravity = 300.0f;

			uint32_t updateFrequency = 50;
			float stepTime = 0;

			bool valid = false;

			std::unordered_map<int, uint32_t> objectIdToIndex; // Game object ID to vector index
			std::vector<DynamicBody> dynamicBodies;
			std::vector<RectCollider> staticColliders;

			SpatialHash hashGrid; // Cell size, max objects, table size
			float broadPhaseRadius = 75.0f;

			std::vector<Vec2<float>> coordsCache;

			bool staticCollidersCached = false;

		};

		struct RayCastData {
			bool hit = false;
			PhysicsObjectRef object;
			Vec2<float> hitEnter, hitExit;

			int32_t getObject() const {
				if (!object.isDynamic) return -1;
				return worldPtr->dynamicBodies[object.vectorIndex].rb.objectId;
			}

			RigidBody* getRigidbody() const {
				if (!object.isDynamic) return nullptr;
				return &worldPtr->dynamicBodies[object.vectorIndex].rb;
			}

			RectCollider* getCollider() const {
				return object.isDynamic ? &worldPtr->dynamicBodies[object.vectorIndex].collider :
					&worldPtr->staticColliders[object.vectorIndex];
			}

		private:
			PhysicsWorld* worldPtr;

			friend class PhysicsEngine;
		};

		void init() override {
			// Debug
			rData.resize(100);
		}

		void createWorld(WorldConfig cfg = {}) {
			world = PhysicsWorld(cfg);
		}

		void addBody(const GameObject& g, RectCollider collider);

		RigidBody* peekRigidBody(const GameObject& g);

		RectCollider* peekCollider(const GameObject& g);

		Vec2<float> getBodyPosition(const GameObject& g);

		void removeBody(const GameObject& g);

		void addStaticCollider(Vec2<int32_t> pos, RectCollider collider, bool update = true);

		void addCollidersFromLayer(const tilemap::ObjectLayer& layer, LayerMask mask = {}, Tag tag = {});

		void update(float dt) override;

		void step();

		std::vector<PhysicsObjectRef>& getObjectsInRadius(Vec2<float> pos, float r);

		std::vector<PhysicsEngine::RayCastData>& rayCast(Vec2<float> rayStart, Vec2<float> rayDirection, float maxRange = 200.0f, uint32_t maxHits = 10);

		void drawGizmos(const graphics::Camera& cam);

		void setDrawGizmos(bool draw) noexcept {
			gizmos = draw;
		}

		PhysicsData getDebugData() const noexcept {
			return pData;
		}

	private:

		// For collecting collision events
		struct CollisionPair {
			int bodyA;
			int bodyB;
			CollisionData dataForA;
			CollisionData dataForB; // Opposite
		};

		void buildHashGrid();

		// Minimum translation vector solver
		void solveCollisions(DynamicBody& body, const std::vector<PhysicsObjectRef>& otherObjects);

		// Contact solver
		void handleCollision(RigidBody& rb, CollisionData& collisionData, bool grounded);

		void collectCallbackEvents(const RigidBody& rb);

		void triggerCallbacks();

		RayCastData slabMethod(Vec2<float> rayStart, Vec2<float> rayDirection, Vec2<float> rayInv, RectCollider collider);

		// Amanatides & Woo
		void rayTraverse(Vec2<float> rayStart, Vec2<float> rayDirection, float maxDist);

		Vec2<float> getMidpoint(Vec2<float> pos, const RectCollider& collider) {
			return { pos.x + collider.left + collider.right * 0.5f, pos.y + collider.top + collider.bottom * 0.5f };
		}

		void updateColliderLastSeen() {
			colliderLastSeen.assign(world.dynamicBodies.size() + world.staticColliders.size(), 0);
		}

		PhysicsWorld world;

		int nextStaticColliderId = std::numeric_limits<int>::max();
		
		std::vector<PhysicsObjectRef> broadPhaseResult;
		std::vector<RayCastData> rayCastResult;

		std::vector<int> colliderLastSeen; // Used to make sure object isn't added multiple times to broad phase result
		int queryGeneration = 0;

		std::vector<CollisionPair> enterEvents;
		std::vector<CollisionPair> exitEvents;
		std::set<std::pair<int, int>> visited;

		// Ray casting
		std::vector<Vec2<int>> traversedCellCoords;

		std::vector<CollisionData> collisionData;

		const float SKIN_WIDTH = 0.4f;

		float timeSinceLastStep = 0.0f;

		// Debugging
		std::vector<RayCastDebugData> rData;
		size_t nextRayIndex = 0;

		bool gizmos = false;
		PhysicsData pData;
		float rayCastLifeTime = 0.2f;
	};
}